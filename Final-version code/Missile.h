#ifndef MODEL_CLASS_IS_INCLUDED
#define MODEL_CLASS_IS_INCLUDED
#include "cannon.h"

const double YsPi = 3.1415927;

class Target
{
public:
	double x, y, z;
};

class Missile
{
	

public:
	double x, y, z, vx, vy, vz;
	double init_x, init_y, init_z;
	int state, count_t;
	Missile();
	Missile(const Missile &incoming);
	~Missile();
	Missile &operator=(const Missile &incoming);

	void initialize(void);
	void setState(const int);
	void setV(double vx, double vy, double vz, double vel);
	int getState(void) const;
	double getX(void) const;
	double getY(void) const;
	double getZ(void) const;
	void computeV(const class Target t, const double &v);
	double collisionT(const class Target t, const double v);
	void Move(double dt);
	void Draw(void) const;
	bool CheckCollision(double cx, double cz, double r1, double r2, double h1, double h2);
};

void Draw_nMissile(int nlive);
void Draw_ReLoad(void);
void Draw_NoMissile(void);
void Draw_Ready(void);

//class CameraObject
//{
//public:
//	double x, y, z;
//	double h, p, b;
//
//	double fov, nearZ, farZ;
//
//	CameraObject();
//	void Initialize(void);
//	void SetUpCameraProjection(void);
//	void SetUpCameraTransformation(void);
//
//	void GetForwardVector(double &vx, double &vy, double &vz);
//};

#endif