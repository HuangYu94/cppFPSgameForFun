#ifndef machineGun_h
#define machineGun_h

#include "fssimplewindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "cannon.h"

const double PI = 3.1415926;

class machineGun
{
protected:
	int shootTime, systemTime1, systemTime2, w, h, status, status1, heat, Number;
	const double damage = 100;

	std::vector<double> bulletX;
	std::vector<double> bulletY;
	std::vector<double> bulletZ;

	std::vector<double> enemyPosition;
	std::vector<double> velocity;

	double p;
	double aspect;


public:
	void Gun(void);
	void setPosition(double headX, double headY, double headZ, double vx, double vy, double vz);
	void shoot(void);
	void render(void);
	void countTime(void);
	void Calculate(void);
	void setwh(void);
	void drawHeat(void);
	bool CheckCollision(double cx, double cz, double r1, double r2, double h1, double h2);
};
#endif