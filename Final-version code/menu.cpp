#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "stlViewer.h"
#include "menu.h"
#include "camera.h"

ability::ability()
{
	n = 4;
    for (int i = 0; i < n; i++)
    {
        abilityList[i] = 250;
    }
}

//ability::~ability()
//{
//    delete[] abilitylist;
//}

void ability::drawAbility()
{
    
    glBegin(GL_QUADS);
    for (int i = 0; i < n; i++)
    {
        glColor3ub(230, 230, 214);
        
        glVertex2i(180      , 450 + 30 * i);
        glVertex2i(abilityList[i]+180, 450 + 30 * i);
        glVertex2i(abilityList[i]+180, 470 + 30 * i);
        glVertex2i(180      , 470 + 30 * i);
        
        glColor3ub(104, 120, 204);
        glVertex2i(180        , 450 + 30 * i);
        glVertex2i(abilityList2[i] + 180, 450 + 30 * i);
        glVertex2i(abilityList2[i] + 180, 470 + 30 * i);
        glVertex2i(180        , 470 + 30 * i);
    }
    glEnd();

	drawButton();

	glColor3ub(0, 0, 0);
	glRasterPos2i(30, 470);
	YsGlDrawFontBitmap8x12("Machine Gun Damage");
	glRasterPos2i(30, 500);
	YsGlDrawFontBitmap8x12("Missile Damage");
	glRasterPos2i(30, 530);
	YsGlDrawFontBitmap8x12("Armor");
	glRasterPos2i(30, 560);
	YsGlDrawFontBitmap8x12("Speed");
}

void drawButton()
{
    glBegin(GL_QUADS);
    glColor3ub(100, 100, 100);
    
    glVertex2i(465, 450);
    glVertex2i(660, 450);
    glVertex2i(660, 500);
    glVertex2i(465, 500);
    
    glVertex2i(465, 510);
    glVertex2i(660, 510);
    glVertex2i(660, 560);
    glVertex2i(465, 560);
    
    glVertex2i(680, 450);
    glVertex2i(780, 450);
    glVertex2i(780, 500);
    glVertex2i(680, 500);
    
    glVertex2i(680, 510);
    glVertex2i(780, 510);
    glVertex2i(780, 560);
    glVertex2i(680, 560);
    
    glEnd();
    glColor3ub(250, 0, 0);
    glRasterPos2i(470, 475);
    YsGlDrawFontBitmap10x14("Upgrade Machine Gun");
    glRasterPos2i(540, 492);
    YsGlDrawFontBitmap8x12("Press:A");
    
    glRasterPos2i(495, 535);
    YsGlDrawFontBitmap10x14("Upgrade Missile");
    glRasterPos2i(540, 552);
    YsGlDrawFontBitmap8x12("Press:B");
    
    glRasterPos2i(710, 475);
    YsGlDrawFontBitmap10x14("Start");
    glRasterPos2i(688, 492);
    YsGlDrawFontBitmap8x12("Press:Enter");
    
    glRasterPos2i(714, 535);
    YsGlDrawFontBitmap10x14("Quit");
    glRasterPos2i(698, 552);
    YsGlDrawFontBitmap8x12("Press:Esc");
    
    glColor3ub(0, 0, 0);
    glRasterPos2i(20, 100);
    YsGlDrawFontBitmap20x32("METAL STORM :MENU");
    
}

StringParser::StringParser()
{
	nw = 0;
	str = nullptr;
	wTop = nullptr;
	wLen = nullptr;
}
StringParser::~StringParser()
{
	CleanUp();
}
void StringParser::CleanUp(void)
{
	nw = 0;
	if (nullptr != str)
	{
		delete[] str;
	}
	if (nullptr != wTop)
	{
		delete[] wTop;
	}
	if (nullptr != wLen)
	{
		delete[] wLen;
	}
}
void StringParser::Parse(char inStr[])
{
	CleanUp();

	auto len = strlen(inStr);

	wTop = new int[1 + len / 2];
	wLen = new int[1 + len / 2];

	str = new char[len + 1];
	strcpy(str, inStr);

	ParseString(nw, wTop, wLen, str, len / 2);
}
int StringParser::size(void)
{
	return nw;
}
void StringParser::GetWord(char wd[], int idx, int maxLen)
{
	wd[0] = 0;
	if (0 <= idx && idx<nw)
	{
		int nCpy = maxLen;
		if (wLen[idx]<nCpy)
		{
			nCpy = wLen[idx];
		}
		strncpy(wd, str + wTop[idx], nCpy);
		wd[nCpy] = 0;
	}
}
//
//Drawing::Drawing()
//{
//    nLine = 0;
//    ln = NULL;
//}
//Drawing::Drawing(const Drawing &incoming)
//{
//    if (incoming.ln != this->ln)
//    {
//        ln = new Line[incoming.nLine];
//        for (int i = 0; i<incoming.nLine; ++i)
//        {
//            ln[i] = incoming.ln[i];
//        }
//        nLine = incoming.nLine;
//    }
//}
//Drawing &Drawing::operator=(const Drawing &incoming)
//{
//    if (incoming.ln != this->ln)
//    {
//        Clear();
//        ln = new Line[incoming.nLine];
//        for (int i = 0; i<incoming.nLine; ++i)
//        {
//            ln[i] = incoming.ln[i];
//        }
//        nLine = incoming.nLine;
//    }
//    return *this;
//}
//Drawing::~Drawing()
//{
//    Clear();
//}
//
//void Drawing::Clear(void)
//{
//    nLine = 0;
//    if (ln != NULL)
//    {
//        delete[] ln;
//        ln = NULL;
//    }
//}
//
//int Drawing::Load(const char fn[])
//{
//    FILE *fp;
//    char str[256];
//    
//    fp = fopen(fn, "r");
//    if (fp != NULL)
//    {
//        Clear();
//        
//        if (fgets(str, 255, fp) != NULL)
//        {
//            int nRead;
//            nLine = atoi(str);
//            ln = new Line[nLine];
//            if (NULL != ln)
//            {
//                nRead = 0;
//                while (fgets(str, 255, fp) != NULL && nRead<nLine)
//                {
//                    StringParser parser;
//                    parser.SetStringAndParse(str);
//                    if (7 <= parser.GetNumWords())
//                    {
//                        char word[256];
//                        parser.GetWord(word, 255, 0);
//                        ln[nRead].x1 = atoi(word);
//                        parser.GetWord(word, 255, 1);
//                        ln[nRead].y1 = atoi(word);
//                        parser.GetWord(word, 255, 2);
//                        ln[nRead].x2 = atoi(word);
//                        parser.GetWord(word, 255, 3);
//                        ln[nRead].y2 = atoi(word);
//                        parser.GetWord(word, 255, 4);
//                        ln[nRead].r = atoi(word);
//                        parser.GetWord(word, 255, 5);
//                        ln[nRead].g = atoi(word);
//                        parser.GetWord(word, 255, 6);
//                        ln[nRead].b = atoi(word);
//                        nRead++;
//                    }
//                }
//            }
//            else
//            {
//                nLine = 0;
//            }
//        }
//        fclose(fp);
//        return 1;
//    }
//    return 0;
//}
//
//void Drawing::Draw(void) const
//{
//    int i;
//    glBegin(GL_LINES);
//    for (i = 0; i<nLine; i++)
//    {
//        glColor3ub(ln[i].r, ln[i].g, ln[i].b);
//        glVertex2i(ln[i].x1, 600 - ln[i].y1);
//        glVertex2i(ln[i].x2, 600 - ln[i].y2);
//    }
//    glEnd();
//}

STL LoadFile(const char fn[])
{
    STL d;
	d.ReadFromFile(fn);
    return d;
}

ability menu::runDraw(int k)
{
    ability ability;
    model model[2];
    model[0].initialize("concorde.stl", 200, 100, 180, 200);
    model[1].initialize("hawk.stl", 150, 150, 120, 120);
    
    STL load = LoadFile(model[k].modelName);
    
    int abilityList2[] = { model[k].missileDamage, model[k].gunDamage, model[k].armor, model[k].speed };

	// 3D drawing from here
	load.Draw();

	ability.getAbility(ability.n, ability.abilityList, abilityList2);

	return ability;

    
    
}

void ability::getAbility(int nn, int list1[4], int list2[4]) {
	n = nn;
	for (int i = 0; i < n; i++) {
		abilityList[i] = list1[i];
		abilityList2[i] = list2[i];
	}
};
const char *menu::returnModelName(int k)
{
	model model[2];
	model[0].initialize("concorde.stl", 200, 100, 180, 200);
	model[1].initialize("hawk.stl", 150, 150, 120, 120);

	if (k == 1) {
		return "shape4.txt";
	}
	else {
		return "shape3.txt";
	}
}

void model::initialize(const char str[], int md, int gd, int a, int s)
{
    modelName = str;
    
    missileDamage = md;
    gunDamage = gd;
    armor = a;
    speed = s;
}





