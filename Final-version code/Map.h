#ifndef Map_h
#define Map_h
#include <stdio.h>
#include "fssimplewindow.h"
#include "yspng.h"

int DetectX(double x);
int DetectY(double y);
int DetectZ(double z);
void DrawGround(void);
void DrawSky(void);
void DrawSurrounding(void);

GLuint ReadImage(char str[]);
void PrintImagexz(GLuint texId,int left,int right,int bottom,int top,int offset_min,int offset_max);
void PrintImagexy(GLuint texId,int left,int right,int bottom,int top,int offset_min,int offset_max);
void PrintImageyz(GLuint texId,int left,int right,int bottom,int top,int offset_min,int offset_max);

#endif
