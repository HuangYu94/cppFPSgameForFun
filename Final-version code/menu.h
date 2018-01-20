#ifndef MENU_IS_INCLUDED
#define MENU_IS_INCLUDED
#include "Plane.h"
#include "stlViewer.h"
class ability
{
public:
	int n;
	int abilityList[4];
	int abilityList2[4];

	ability();
	//ability(ability &another);
	//~ability();
	void getAbility(int, int[4], int[4]);
	void drawAbility();

};

void drawButton(void);

class StringParser
{
protected:
	int nw;
	char *str;
	int *wTop, *wLen;

public:
	StringParser();
	~StringParser();
	void CleanUp(void);

	void Parse(char inStr[]);

	int size(void);
	void GetWord(char wd[], int idx, int maxLen);
};

class Line
{
public:
	int x1, y1, x2, y2;
	int r, g, b;
};

//class Drawing
//{
//protected:
//	int nLine;
//	Line *ln;
//public:
//	Drawing();
//	Drawing(const Drawing &incoming);
//	Drawing &operator=(const Drawing &incoming);
//	~Drawing();
//	void Clear(void);
//	int Load(const char fn[]);
//	void Draw(void) const;
//};

STL LoadFile(const char fn[]);

class menu
{
public:
	int key;
	ability runDraw(int k);
	const char *returnModelName(int k);


};

class model
{
public:
	ability abil;
	const char *modelName;

	int missileDamage;
	int gunDamage;
	int armor;
	int speed;

	void initialize(const char str[], int md, int gd, int a, int s);

};


#endif
#pragma once
