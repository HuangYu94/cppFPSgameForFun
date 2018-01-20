#include <stdio.h>

#define _WINSOCKAPI_
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include "yssimplesound.h"


#pragma comment(lib,"user32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"dsound.lib")


class YsSoundPlayer::APISpecificData
{
public:
	/*!  Direct Sound API is so badly designed that requires a window handle to play a sound.
	     Yes, you hear it right.  A window handle is a mandatory parameter, not an option!
	     Why the hell a sound API requires a window handle?
	     Evidently, there is  at least one unqualified programmer working for Microsoft,
	     at a very important position that makes decision of a spec of a sound API,
	     presumably earning more than triple income than my salary.

	     Because of that unqualified programmer, I need to write a class to find (or guess) 
	     a main-window handle of the application.
	     It works for a single-window application, which I make most of the time.
	     But, it may not work if the application uses multiple windows.
	*/
	class MainWindowFinder
	{
	public:
		/*! This function finds a handle of the largest window that is visible and is associated with the current process Id.
		    This window may not be an application main window.  But, what else can I do?
		    This function can be used for giving a window handle to a badly-designed APIs such as Direct Sound API.
		*/
		static HWND Find(void);
	private:
		static void SearchTopLevelWindow(HWND &hWndLargest,int &largestWndArea,HWND hWnd,DWORD procId);
	};


	HWND hWndMain;
	LPDIRECTSOUND8 dSound8;

	APISpecificData();
	~APISpecificData();
	void CleanUp(void);
	void Start(void);
	void End(void);
};

class YsSoundPlayer::SoundData::APISpecificDataPerSoundData
{
public:
	LPDIRECTSOUNDBUFFER dSoundBuf;

	APISpecificDataPerSoundData();
	~APISpecificDataPerSoundData();
	void CleanUp(void);

	void CreateBuffer(LPDIRECTSOUND8 dSound8,SoundData &dat);
};



////////////////////////////////////////////////////////////



// Source: http://stackoverflow.com/questions/6202547/win32-get-main-wnd-handle-of-application
HWND YsSoundPlayer::APISpecificData::MainWindowFinder::Find(void)
{
	HWND hWndLargest=NULL;
	int wndArea=0;
	SearchTopLevelWindow(hWndLargest,wndArea,NULL,GetCurrentProcessId());
	return hWndLargest;
}
void YsSoundPlayer::APISpecificData::MainWindowFinder::SearchTopLevelWindow(HWND &hWndLargest,int &largestWndArea,HWND hWnd,DWORD procId)
{
	DWORD windowProcId;
	GetWindowThreadProcessId(hWnd,&windowProcId);
	if(windowProcId==procId)
	{
		char str[256];
		GetWindowTextA(hWnd,str,255);
		printf("hWnd=%08x Title=%s\n",(int)hWnd,str);
		printf("IsVisible=%d\n",IsWindowVisible(hWnd));

		RECT rc;
		GetWindowRect(hWnd,&rc);

		int area=((rc.right-rc.left)*(rc.bottom-rc.top));
		if(0>area)
		{
			area=-area;
		}
		printf("Area=%d square pixels. (%d x %d)\n",area,(rc.right-rc.left),(rc.bottom-rc.top));

		if(TRUE==IsWindowVisible(hWnd) && (NULL==hWndLargest || largestWndArea<area))
		{
			hWndLargest=hWnd;
			largestWndArea=area;
		}
		return;
	}

	HWND hWndChild=NULL;
	while(NULL!=(hWndChild=FindWindowEx(hWnd,hWndChild,NULL,NULL))!=NULL)
	{
		SearchTopLevelWindow(hWndLargest,largestWndArea,hWndChild,procId);
	}
}

////////////////////////////////////////////////////////////



YsSoundPlayer::APISpecificData::APISpecificData()
{
	hWndMain=nullptr;
	dSound8=nullptr;
	CleanUp();
}
YsSoundPlayer::APISpecificData::~APISpecificData()
{
	CleanUp();
}

void YsSoundPlayer::APISpecificData::CleanUp(void)
{
	hWndMain=nullptr;
	if(nullptr!=dSound8)
	{
		dSound8->Release();
		dSound8=nullptr;
	}
}

void YsSoundPlayer::APISpecificData::Start(void)
{
	CleanUp();

	MainWindowFinder mainWindowFinder;
	hWndMain=mainWindowFinder.Find();
	if(DS_OK==DirectSoundCreate8(NULL,&dSound8,NULL))
	{
		dSound8->SetCooperativeLevel(hWndMain,DSSCL_PRIORITY);
	}
}
void YsSoundPlayer::APISpecificData::End(void)
{
	CleanUp();
}

////////////////////////////////////////////////////////////

YsSoundPlayer::APISpecificData *YsSoundPlayer::CreateAPISpecificData(void)
{
	return new APISpecificData;
}
void YsSoundPlayer::DeleteAPISpecificData(APISpecificData *ptr)
{
	delete ptr;
}

YSRESULT YsSoundPlayer::StartAPISpecific(void)
{
	api->Start();
	return YSOK;
}
YSRESULT YsSoundPlayer::EndAPISpecific(void)
{
	api->End();
	return YSOK;
}

YSRESULT YsSoundPlayer::PlayOneShotAPISpecific(SoundData &dat)
{
	if(nullptr!=dat.api->dSoundBuf)
	{
		dat.api->dSoundBuf->SetCurrentPosition(0);
		dat.api->dSoundBuf->Play(0,0xc0000000,0);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::PlayBackgroundAPISpecific(SoundData &dat)
{
	if(nullptr!=dat.api->dSoundBuf)
	{
		DWORD sta;
		dat.api->dSoundBuf->GetStatus(&sta);
		if(0==(sta&DSBSTATUS_PLAYING))
		{
			dat.api->dSoundBuf->SetCurrentPosition(0);
			dat.api->dSoundBuf->Play(0,0xc0000000,DSBPLAY_LOOPING);
		}
		return YSOK;
	}
	return YSERR;
}

YSBOOL YsSoundPlayer::IsPlayingAPISpecific(const SoundData &dat) const
{
	if(nullptr!=dat.api->dSoundBuf)
	{
		DWORD sta;
		dat.api->dSoundBuf->GetStatus(&sta);
		if(0!=(sta&DSBSTATUS_PLAYING))
		{
			return YSTRUE;
		}
	}
	return YSFALSE;
}

void YsSoundPlayer::StopAPISpecific(SoundData &dat)
{
	if(nullptr!=dat.api->dSoundBuf)
	{
		dat.api->dSoundBuf->Stop();
		// Memo: IDirectSoundBuffer::Stop() actually pauses playing.
		//       It does not rewind.  Therefore, to play from the head of the wave,
		//       IDirectSoundBuffer::SetCurrentPosition(0) must be called before playing.
	}
}

void YsSoundPlayer::KeepPlayingAPISpecific(void)
{
}

////////////////////////////////////////////////////////////

YsSoundPlayer::SoundData::APISpecificDataPerSoundData::APISpecificDataPerSoundData()
{
	dSoundBuf=nullptr;
	CleanUp();
}
YsSoundPlayer::SoundData::APISpecificDataPerSoundData::~APISpecificDataPerSoundData()
{
	CleanUp();
}
void YsSoundPlayer::SoundData::APISpecificDataPerSoundData::CleanUp(void)
{
	if(nullptr!=dSoundBuf)
	{
		dSoundBuf->Release();
		dSoundBuf=nullptr;
	}
}

void YsSoundPlayer::SoundData::APISpecificDataPerSoundData::CreateBuffer(LPDIRECTSOUND8 dSound8,SoundData &dat)
{
	CleanUp();

	const int nChannels=dat.GetNumChannel();
	const int nBlockAlign=nChannels*dat.BitPerSample()/8;
	const int nAvgBytesPerSec=dat.PlayBackRate()*nBlockAlign;

	WAVEFORMATEX fmt;
	fmt.cbSize=sizeof(fmt);
	fmt.wFormatTag=WAVE_FORMAT_PCM;

	fmt.nChannels=nChannels;
	fmt.nSamplesPerSec=dat.PlayBackRate();
	fmt.wBitsPerSample=dat.BitPerSample();

	fmt.nBlockAlign=nBlockAlign;
	fmt.nAvgBytesPerSec=nAvgBytesPerSec;


	DSBUFFERDESC desc;
	desc.dwSize=sizeof(desc);
	desc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_LOCDEFER;
	desc.dwBufferBytes=dat.SizeInByte();
	desc.dwReserved=0;
	desc.lpwfxFormat=&fmt;
	desc.guid3DAlgorithm=GUID_NULL;
	if(DS_OK==dSound8->CreateSoundBuffer(&desc,&dSoundBuf,NULL))
	{
		auto datPtr=dat.DataPointer();

		DWORD writeBufSize1,writeBufSize2;
		unsigned char *writeBuf1,*writeBuf2;
		if(dSoundBuf->Lock(0,0,(LPVOID *)&writeBuf1,&writeBufSize1,(LPVOID *)&writeBuf2,&writeBufSize2,DSBLOCK_ENTIREBUFFER)==DS_OK &&
		   NULL!=writeBuf1)
		{
			printf("Buffer Locked\n");

			for(int i=0; i<(int)dat.SizeInByte() && i<(int)writeBufSize1; i++)
			{
				writeBuf1[i]=datPtr[i];
			}

			dSoundBuf->Unlock(writeBuf1,writeBufSize1,writeBuf2,writeBufSize2);
		}
		else
		{
			printf("Failed to Lock Buffer.\n");
			CleanUp();
		}
	}
	
}

////////////////////////////////////////////////////////////

YsSoundPlayer::SoundData::APISpecificDataPerSoundData *YsSoundPlayer::SoundData::CreateAPISpecificData(void)
{
	return new APISpecificDataPerSoundData;
}
void YsSoundPlayer::SoundData::DeleteAPISpecificData(APISpecificDataPerSoundData *ptr)
{
	delete ptr;
}

YSRESULT YsSoundPlayer::SoundData::PreparePlay(YsSoundPlayer &player)
{
	if(nullptr!=api->dSoundBuf)
	{
		return YSOK;
	}

	api->CreateBuffer(player.api->dSound8,*this);
	if(nullptr!=api->dSoundBuf)
	{
		return YSOK;
	}
	return YSERR;
}

void YsSoundPlayer::SoundData::CleanUpAPISpecific(void)
{
	if(nullptr!=playerStatePtr && YsSoundPlayer::STATE_ENDED==*playerStatePtr)
	{
		// In this case, DirectSoundBuffer is gone with the player.
		api->dSoundBuf=nullptr;
	}
	api->CleanUp();
}
