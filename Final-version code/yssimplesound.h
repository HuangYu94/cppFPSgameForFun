#ifndef YSSIMPLESOUND_IS_INCLUDED
#define YSSIMPLESOUND_IS_INCLUDED
/* { */


#include <vector>
#include <memory>


#ifndef YSRESULT_IS_DEFINED
#define YSRESULT_IS_DEFINED
/*! Enum for processing result. */
typedef enum
{
	YSERR,  /*!< There were error(s). */
	YSOK    /*!< The process was successful. */
} YSRESULT;
#endif

#ifndef YSBOOL_IS_DEFINED
#define YSBOOL_IS_DEFINED
/*! Enum for boolearn. */
typedef enum
{
	YSFALSE,     /*!< False */
	YSTRUE,      /*!< True */
	YSTFUNKNOWN  /*!< Unable to tell true or false. */
} YSBOOL;
#endif


/*! This class plays a .WAV format sound.

    To compile with CMake, add a common library with target_link_library:
      - yssimplesound
    and, one of the following platform-specific library:
      - yssimplesound_macosx  (MacOSX)
      - yssimplesound_linux_alsa  (Linux + ALSA)
      - yssimplesound_dsound  (Windows + DirectSound)
      - yssimplesound_nownd
    The last one, yssimplesound_nownd, is for a platform that is not yet supported, but
    at least allow linking the library to make an executable.

    To compile without CMake, add the following .CPP and .H files in the project.

    For MacOSX (Require Cocoa framework)
      yssimplesound.cpp
      yssimplesound.h
      macosx/yssimplesound_macosx_cpp.cpp
      macosx/yssimplesound_macosx_objc.m      

    For Linux (Require ALSA library, libasound.a)
      yssimplesound.cpp
      yssimplesound.h
      linux_alsa/yssimplesound_linux_alsa.cpp

    For Windows (Direct Sound):
      yssimplesound.cpp
      yssimplesound.h
      windows_dsound/yssimplesound_dsound.cpp

    For No Window (No sound, linking only)
      yssimplesound.cpp
      yssimplesound.h
      nownd/yssimplesound_nownd.cpp


  Unfortunately, whoever designed DirectSound API didn't know about the basics of programming.
  DirectSound API is so poorly designed that it requires a window to play a sound.
  Unnecessary dependencies are one of the worst enemy in programming, and that person obviously didn't know about it.

  Yeah, yeah.  You can say the sound should stop when the window is not active.  
  Then, rather than making it a requirement, it can add an option to stop playing when the
  associated window is not active.  No matter what excuse they give, DirectSound is a textbook example of failed-design.

  Therefore, you cannot write a window-less test/sample program that plays a sound.
  I don't understand how that unqualified person ever got a position in Microsoft and end up designing such an important API.
  I suppose they have a serious problem in human resource.

*/
class YsSoundPlayer
{
private:
	static YsSoundPlayer *currentPlayer;

public:
	enum STATE
	{
		STATE_UNINITIALIZED,
		STATE_STARTED,
		STATE_ENDED
	};

	class SoundData;
	std::shared_ptr <STATE> playerStatePtr;

private:
	class APISpecificData;

	SoundData *background;
	APISpecificData *api;

	// Written per API >>
	APISpecificData *CreateAPISpecificData(void);
	void DeleteAPISpecificData(APISpecificData *);
	// Written per API <<

public:
	YsSoundPlayer();
	~YsSoundPlayer();

	void MakeCurrent(void);
	static void NullifyCurrentPlayer(void);
	static YsSoundPlayer *GetCurrentPlayer(void);

	// Written per API >>
	YSRESULT StartAPISpecific(void);
	YSRESULT EndAPISpecific(void);
	YSRESULT PlayOneShotAPISpecific(SoundData &dat); // Maybe oneShot is still playing.  This API may be able to play multiple WAV simultaneously.
	YSRESULT PlayBackgroundAPISpecific(SoundData &dat);
	void StopAPISpecific(SoundData &dat);           // It is guaranteed that oneShot is not nullptr when called.
	void KeepPlayingAPISpecific(void);
	YSBOOL IsPlayingAPISpecific(const SoundData &dat) const;
	// Written per API <<

	void Start(void);
	void End(void);
	void PlayOneShot(SoundData &dat);
	void PlayBackground(SoundData &dat);
	void Stop(SoundData &dat);
	void KeepPlaying(void);
	YSBOOL IsPlaying(const SoundData &dat) const;
};



class YsSoundPlayer::SoundData
{
private:
	friend class YsSoundPlayer;
	class APISpecificDataPerSoundData;
	std::shared_ptr <YsSoundPlayer::STATE> playerStatePtr;

	YSBOOL stereo;
	unsigned int bit;
	unsigned int rate;
	unsigned int sizeInBytes;

	YSBOOL isSigned;
	unsigned char *dat;

	// Written per API >>
	APISpecificDataPerSoundData *api;
	// Written per API <<

	class MemInStream
	{
	private:
		long long int length;
		long long int pointer;
		const unsigned char *dat;
	public:
		MemInStream(long long int len,const unsigned char dat[]);
		long long int Fetch(unsigned char buf[],long long int len);
		long long int Skip(long long int len);
	};


	APISpecificDataPerSoundData *CreateAPISpecificData(void);
	void DeleteAPISpecificData(APISpecificDataPerSoundData *ptr);
	
public:
	SoundData();
	~SoundData();
	void Initialize(void);

	void CleanUp(void);

	unsigned int NTimeStep(void) const;

	YSBOOL Stereo(void) const;
	unsigned int BytePerTimeStep(void) const;
	unsigned int BitPerSample(void) const;
	unsigned int BytePerSample(void) const;
	unsigned int PlayBackRate(void) const;
	unsigned int SizeInByte(void) const;
	YSBOOL IsSigned(void) const;
	const unsigned char *DataPointer(void) const;
	const unsigned char *DataPointerAtTimeStep(unsigned int ts) const;

	YSRESULT LoadWav(const char fn[]);
	YSRESULT LoadWavFromMemory(long long int length,const unsigned char dat[]);
	YSRESULT ConvertTo16Bit(void);
	YSRESULT ConvertTo8Bit(void);
	YSRESULT ConvertToStereo(void);
	YSRESULT ConvertToMono(void);
	YSRESULT Resample(int newRate);

	YSRESULT ConvertToSigned(void);
	YSRESULT ConvertToUnsigned(void);



	std::vector <unsigned char> MakeWavByteData(void) const;
	static void AddUnsignedInt(std::vector <unsigned char> &byteData,unsigned int dat);
	static void AddUnsignedShort(std::vector <unsigned char> &byteData,unsigned short dat);



private:
	int GetNumChannel(void) const;
	int GetNumSample(void) const;
	int GetNumSamplePerChannel(void) const;
	size_t GetUnitSize(void) const;
	size_t GetSamplePosition(int atIndex) const;
	int GetSignedValue(int atTimeStep,int channel) const;
	void SetSignedValue(unsigned char *savePtr,int rawSignedValue);

	// Written per API >>
public:
	YSRESULT PreparePlay(YsSoundPlayer &player);
private:
	void CleanUpAPISpecific(void);
	// Written per API <<
};


/* } */
#endif
