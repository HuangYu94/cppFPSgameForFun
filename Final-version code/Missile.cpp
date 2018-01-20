#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Missile.h"
#include "cannon.h"

Missile::Missile()
{
	initialize();
}

Missile::Missile(const Missile & incoming)
{
	x = incoming.x;
	y = incoming.y;
	z = incoming.z;
	vx = incoming.vx;
	vy = incoming.vy;
	vz = incoming.vz;
	init_x = incoming.init_x;
	init_y = incoming.init_y;
	init_z = incoming.init_z;
	state = incoming.state;
	count_t = incoming.count_t;
}

Missile::~Missile()
{
	x = 0.0, y = 0.0, z = 0.0;
	init_x = 0.0, init_y = 0.0, init_z = 0.0;
	vx = 0.0, vy = 0.0, vz = 0.0;
	state = 0, count_t = 0;
}

Missile & Missile::operator=(const Missile & incoming)
{
	x = incoming.x;
	y = incoming.y;
	z = incoming.z;
	vx = incoming.vx;
	vy = incoming.vy;
	vz = incoming.vz;
	init_x = incoming.init_x;
	init_y = incoming.init_y;
	init_z = incoming.init_z;
	state = incoming.state;
	count_t = incoming.count_t;

	return *this;
}

void Missile::initialize(void)
{
	x = 20.0, y = 20.0, z = 20.0;
	init_x = x, init_y = y, init_z = z;
	vx = 0.0, vy = 0.0, vz = 0.0;
	state = 0, count_t = 0;
}

void Missile::setState(const int s)
{
	state = s;
}

int Missile::getState(void) const
{
	return state;
}

double Missile::getX(void) const
{
	return x;
}

double Missile::getY(void) const
{
	return y;
}

double Missile::getZ(void) const
{
	return z;
}

void Missile::computeV(const class Target t, const double &v)
{
	double dx = x - t.x;
	double dy = y - t.y;
	double dz = z - t.z;

	double d = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	vx = (dx / d) * v;
	vy = (dy / d) * v;
	vz = (dz / d) * v;
}

double Missile::collisionT(const Target t, const double v)
{
	double dx = init_x - t.x;
	double dy = init_y - t.y;
	double dz = init_z - t.z;

	double d = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	return d / v;
}

void Missile::Move(double dt)
{
	if (y>0)
	{
		x += vx*dt*2;
		y += vy*dt*2;
		z += vz*dt*2;
		//count_t++;
	}
	else
	{
		vx = 0.0, vy = 0.0, vz = 0.0;
		setState(0);
	}
}

void Missile::setV(double vx_in, double vy_in, double vz_in, double vel) {
	vx = vx_in*vel;
	vy = vy_in*vel;
	vz = vz_in*vel;
}

bool Missile::CheckCollision(double cx, double cz, double r1, double r2, double h1, double h2)
{
	if (h1 < y && h1 + h2 > y)
	{
		for (int i = 0; i < 10; i++)
		{
			double d2 = pow(fabs(x - cx), 2) + pow(fabs(z - cz), 2);
			if (d2 < pow(r2, 2))
			{
				return true;
			}
		}
	}
	else if (h1 >= y && 0 < y)
	{
		for (int i = 0; i < 10; i++)
		{
			double d2 = pow(fabs(x - cx), 2) + pow(fabs(z - cz), 2);
			if (d2 < pow(r1, 2))
			{
				return true;
			}
		}
	}
	return false;
}

void Draw_nMissile(int nlive)
{
	char buffer[20];
	char *num = itoa(nlive, buffer, 10);
	char str[80];
	strcpy(str, "Missile Remaining : ");
	strcat(str, num);
	glColor3ub(255, 0, 0);
	glRasterPos2i(16, 32);
	YsGlDrawFontBitmap12x16(str);
}

void Draw_ReLoad(void)
{
	char str[80];
	strcpy(str, "Reloading!!!");
	glColor3ub(255, 0, 0);
	glRasterPos2i(32*10, 32*5);
	YsGlDrawFontBitmap12x16(str);
}

void Draw_NoMissile(void)
{
	char str[80];
	strcpy(str, "No Missile!!!");
	glColor3ub(255, 0, 0);
	glRasterPos2i(32*10, 32*5);
	YsGlDrawFontBitmap12x16(str);
}

void Draw_Ready(void)
{
	char str[80];
	strcpy(str, "Missile Ready!!!");
	glColor3ub(255, 0, 0);
	glRasterPos2i(32 * 10, 32 * 5);
	YsGlDrawFontBitmap12x16(str);
}

//CameraObject::CameraObject()
//{
//    Initialize();
//}
//
//void CameraObject::Initialize(void)
//{
//    x=0;
//    y=0;
//    z=0;
//    h=0;
//    p=0;
//    b=0;
//
//    fov=YsPi/6.0;  // 30 degree
//    nearZ=0.1;
//    farZ=200.0;
//}
//
//void CameraObject::SetUpCameraProjection(void)
//{
//    int wid,hei;
//    double aspect;
//
//    FsGetWindowSize(wid,hei);
//    aspect=(double)wid/(double)hei;
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(fov*180.0/YsPi,aspect,nearZ,farZ);
//}
//
//void CameraObject::SetUpCameraTransformation(void)
//{
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glRotated(-b*180.0/YsPi,0.0,0.0,1.0);  // Rotation about Z axis
//    glRotated(-p*180.0/YsPi,1.0,0.0,0.0);  // Rotation about X axis
//    glRotated(-h*180.0/YsPi,0.0,1.0,0.0);  // Rotation about Y axis
//    glTranslated(-x,-y,-z);
//}
//
//void CameraObject::GetForwardVector(double &vx,double &vy,double &vz)
//{
//    vx=-cos(p)*sin(h);
//    vy= sin(p);
//    vz=-cos(p)*cos(h);
//}



void Missile::Draw(void) const
{
	double r =0.5;
	glBegin(GL_QUADS);
	//printf("missile position: %1.4f %1.4f %1.4f\n", x, y, z);
	//printf("missile velocity: %1.4f %1.4f %1.4f\n", vx, vy, vz);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3d(x - r, y - r, z - r);
	glVertex3d(x + r, y - r, z - r);
	glVertex3d(x + r, y + r, z - r);
	glVertex3d(x - r, y + r, z - r);

	glVertex3d(x - r, y - r, z + r);
	glVertex3d(x + r, y - r, z + r);
	glVertex3d(x + r, y + r, z + r);
	glVertex3d(x - r, y + r, z + r);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3d(x - r, y - r, z - r);
	glVertex3d(x + r, y - r, z - r);
	glVertex3d(x + r, y - r, z + r);
	glVertex3d(x - r, y - r, z + r);

	glVertex3d(x - r, y + r, z - r);
	glVertex3d(x + r, y + r, z - r);
	glVertex3d(x + r, y + r, z + r);
	glVertex3d(x - r, y + r, z + r);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3d(x - r, y - r, z - r);
	glVertex3d(x - r, y + r, z - r);
	glVertex3d(x - r, y + r, z + r);
	glVertex3d(x - r, y - r, z + r);

	glVertex3d(x + r, y - r, z - r);
	glVertex3d(x + r, y + r, z - r);
	glVertex3d(x + r, y + r, z + r);
	glVertex3d(x + r, y - r, z + r);

	glEnd();
}

