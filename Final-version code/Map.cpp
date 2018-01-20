#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "Map.h"
#include "fssimplewindow.h"
#include "yspng.h"

const double h_min = -1000.0;
const double h_max = 1000.0;
const double v_min = 0.0;
const double v_max = 500.0;

//void DrawGround(void)
//{
//    glColor3f(0.0f,0.0f,1.0f);
//    glBegin(GL_LINES);
//    for(int x=h_min; x<=h_max; x+=20)
//    {
//        glVertex3i(x,v_min,h_min);
//        glVertex3i(x,v_min, h_max);
//        
//        glVertex3i(h_min,v_min,x);
//        glVertex3i(h_max,v_min,x);
//    }
//    glEnd();
//}
//
//void DrawSky(void)
//{
//    glColor3f(0.0f,0.0f,1.0f);
//    glBegin(GL_LINES);
//    for(int x=h_min; x<=h_max; x+=20)
//    {
//        glVertex3i(x,v_max,h_min);
//        glVertex3i(x,v_max, h_max);
//        
//        glVertex3i(h_min,v_max,x);
//        glVertex3i(h_max,v_max,x);
//    }
//    glEnd();
//}
//
//void DrawSurrounding(void)
//{
//    glColor3f(0.0f,0.0f,1.0f);
//    glBegin(GL_LINES);
//    for(int x=h_min; x<=h_max; x+=20)
//    {
//        glVertex3i(x,v_max,h_max);
//        glVertex3i(x,v_min, h_max);
//        
//        glVertex3i(x,v_max,h_min);
//        glVertex3i(x,v_min,h_min);
//        
//        glVertex3i(h_max,v_max,x);
//        glVertex3i(h_max,v_min,x);
//        
//        glVertex3i(h_min,v_max,x);
//        glVertex3i(h_min,v_min,x);
//    }
//    for(int y=v_min; y<=v_max; y+=20)
//    {
//        glVertex3i(h_max,y,h_max);
//        glVertex3i(h_min,y, h_max);
//        
//        glVertex3i(h_max,y,h_min);
//        glVertex3i(h_min,y, h_min);
//        
//        glVertex3i(h_max,y,h_max);
//        glVertex3i(h_max,y,h_min);
//        
//        glVertex3i(h_min,y,h_max);
//        glVertex3i(h_min,y,h_min);
//    }
//    glEnd();
//}

int DetectX(double x){
    if (x<=h_min+10){
        return h_min+10;
    }
    else if(x>=h_max-10){
        return h_max-10;
    }
    else{
        return x;
    }
}

int DetectY(double y){
    if (y<=v_min+10){
        return v_min+10;
    }
    else if(y>=v_max-10){
        return v_max-10;
    }
    else{
        return y;
    }
}
int DetectZ(double z){
    if (z<=h_min+10){
        return h_min+10;
    }
    else if(z>=h_max-10){
        return h_max-10;
    }
    else{
        return z;
    }
}

GLuint ReadImage(char str[]){
    YsRawPngDecoder ground;
    ground.Decode(str);
    
    GLuint texId;
    glGenTextures(1,&texId);
    glBindTexture(GL_TEXTURE_2D,texId);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    glTexImage2D
    (GL_TEXTURE_2D,
     0,    // Level of detail
     GL_RGBA,
     ground.wid,
     ground.hei,
     0,    // Border width, but not supported and needs to be 0.
     GL_RGBA,
     GL_UNSIGNED_BYTE,
     ground.rgba);
    
    return texId;
}

void PrintImagexz(GLuint texId,int left,int right,int bottom,int top,int offset_min,int offset_max){
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D,texId);
    
    glBegin(GL_QUADS);
    
    glTexCoord2d(0.0,0.0);
    glVertex3i(left,offset_min,bottom);
    
    glTexCoord2d(1.0,0.0);
    glVertex3i(left,offset_max,top);
    
    glTexCoord2d(1.0,1.0);
    glVertex3i(right,offset_max,top);
    
    glTexCoord2d(0.0,1.0);
    glVertex3i(right,offset_min,bottom);
    
    glEnd();
}

void PrintImagexy(GLuint texId,int left,int right,int bottom,int top,int offset_min,int offset_max){
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D,texId);
    
    glBegin(GL_QUADS);
    
    glTexCoord2d(0.0,0.0);
    glVertex3i(left,offset_min,bottom);
    
    glTexCoord2d(1.0,0.0);
    glVertex3i(left,offset_max,top);
    
    glTexCoord2d(1.0,1.0);
    glVertex3i(right,offset_max,top);
    
    glTexCoord2d(0.0,1.0);
    glVertex3i(right,offset_min,bottom);
    
    glEnd();
}

void PrintImageyz(GLuint texId,int left,int right,int bottom,int top,int offset_min,int offset_max){
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D,texId);
    
    glBegin(GL_QUADS);
    
    glTexCoord2d(0.0,0.0);
    glVertex3i(left,offset_min,bottom);
    
    glTexCoord2d(1.0,0.0);
    glVertex3i(left,offset_max,bottom);
    
    glTexCoord2d(1.0,1.0);
    glVertex3i(right,offset_max,top);
    
    glTexCoord2d(0.0,1.0);
    glVertex3i(right,offset_min,top);
    
    glEnd();
}
