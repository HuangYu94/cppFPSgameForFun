#include "machineGun.h"
#include "Missile.h"

class CameraObject
{
public:
	double x, y, z;
	double h, p, b;

	double fov, nearZ, farZ;

	CameraObject();
	void Initialize(void);
	void SetUpCameraProjection(void);
	void SetUpCameraTransformation(void);

	void GetForwardVector(double &vx, double &vy, double &vz);
};

CameraObject::CameraObject()
{
	Initialize();
}

void CameraObject::Initialize(void)
{
	x = 0;
	y = 0;
	z = 0;
	h = 0;
	p = 0;
	b = 0;

	fov = YsPi / 6.0;  // 30 degree
	nearZ = 0.1;
	farZ = 2000.0;
}

void CameraObject::SetUpCameraProjection(void)
{
	int wid, hei;
	double aspect;

	FsGetWindowSize(wid, hei);
	aspect = (double)wid / (double)hei;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov*180.0 / YsPi, aspect, nearZ, farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(-b*180.0 / YsPi, 0.0, 0.0, 1.0);  // Rotation about Z axis
	glRotated(-p*180.0 / YsPi, 1.0, 0.0, 0.0);  // Rotation about X axis
	glRotated(-h*180.0 / YsPi, 0.0, 1.0, 0.0);  // Rotation about Y axis
	glTranslated(-x, -y, -z);
}

void CameraObject::GetForwardVector(double &vx, double &vy, double &vz)
{
	vx = -cos(p)*sin(h);
	vy = sin(p);
	vz = -cos(p)*cos(h);
}