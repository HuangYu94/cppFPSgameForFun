#ifndef cannon_h
#define cannon_h
#include "Plane.h"

class cMissile
{
public:
	double x, y, z, v;
	int state;
	double alpha, theta;
	int l;
	double damage;

	bool checkHitPlane(double px, double py, double pz, double pBounding);
	void initialize(void);
	void move(void);
	void renderMissile(void);
};

class Cannon
{
public:
	int r1, r2, cl;
	double h1, h2;
	double alpha[10], theta[10];
	int x[10], z[10];
	int state[10];
	int health[10];
	double jx, jy, jz;

	Cannon();
	
	void initialize(void);
	bool checkPlaneColision(Vec3d planeCtr, int cannonIdx);
	void renderCannon(void) const;
	void getJetpos(double x, double y, double z);
	void aim(void);
	void reduceHealth(int i, bool hit);
	void calculate_health(int index);
	void set_state(int index);
};



#endif // !cannon_h
#pragma once
   