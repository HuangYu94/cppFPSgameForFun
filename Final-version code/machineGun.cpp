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

void machineGun::drawHeat(void)
{
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)w - 1, (float)h - 1, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
*/
	glBegin(GL_QUADS);

	glColor3f(1, 0, 0);
	glVertex2d(100, 500);
	glVertex2d(120, 500);
	glVertex2d(120, 500 - heat * 2);
	glVertex2d(100, 500 - heat * 2);

	glEnd();
}

// put the info of enemy position into the class


// set screen size
void machineGun::setwh(void)
{
	int wid, hei;
	FsGetWindowSize(wid, hei);
	aspect = (double)wid / (double)hei;
	w = wid; h = hei;
	status = 0;
	status1 = 0;
	heat = 0;
	Number = 0;
}

//call shoot function and draw all the bullet (which should be set even if the machine gun is not used).
void machineGun::Gun(void)
{
	if (Number != 0)
	{
		shoot();
	}
}

//HERE NEED DISCUSSION: I can change drawing criteria to stop drawing the bullet.
void machineGun::Calculate(void)
{

	for (int i = 0; i < Number; i++)
	{
		bulletX[i] = bulletX[i] + velocity[i * 3] * 3;
		bulletY[i] = bulletY[i] + velocity[i * 3 + 1] * 3;
		bulletZ[i] = bulletZ[i] + velocity[i * 3 + 2] * 3;
	}

	for (int i = 0; i < Number; i++)
	{

		if (bulletY[i] < 0 || bulletY[i]>500)
		{
			bulletX.erase(bulletX.begin() + i);
			bulletY.erase(bulletY.begin() + i);
			bulletZ.erase(bulletZ.begin() + i);

			velocity.erase(velocity.begin() + i * 3, velocity.begin() + i * 3 + 3);
			Number = Number - 1;
		}
	}
	Number = size(bulletX);
}

// set the position of the plane (angle here is the angle which the plane is facing with respect to the normal plane for each bullet (not the view angle)
void machineGun::setPosition(double headX, double headY, double headZ, double vx, double vy, double vz)
{
	if (status != 2)
	{
		int Number2 = size(velocity);

		bulletX.resize(Number + 1);
		bulletY.resize(Number + 1);
		bulletZ.resize(Number + 1);
		velocity.resize(Number2 + 3);

		bulletX[Number] = headX;
		bulletY[Number] = headY;
		bulletZ[Number] = headZ;

		velocity[Number2] = vx;
		velocity[Number2 + 1] = vy;
		velocity[Number2 + 2] = vz;

		heat = heat + 25;
		Number = size(bulletX);
	}

	if (status1 == 0 && status == 0)
	{
		status = 1;
		status1 = 1;
	}
}


void machineGun::shoot(void)
{
	countTime();
	render();
	status1 = 0;
}

void machineGun::countTime(void)
{

	//if (status == 1 || status == 2)
	//{
	heat = heat - 8;
	if (heat < 0)
	{
		heat = 0;
	}
	//}

	if (status == 2 && heat <= 15)
	{
		status = 0;
		status1 = 0;
	}

	if (status == 1 && heat > 200)
	{
		status = 2;
		status1 = 0;
	}
}

// NEED DISCUSSION: translated number (here I view the bullet which shoot from (0,0,0) at (0,2,5);
//draw the bullet 
void machineGun::render()
{
	glLineWidth(6.0);
	glBegin(GL_LINES);

	glColor3ub(255, 0, 0);
	
	for (int i = 0; i < Number; i++)
	{

		glVertex3d(bulletX[i], bulletY[i], bulletZ[i]);
		glVertex3d(bulletX[i] - velocity[i * 3] * 5, bulletY[i] - velocity[i * 3 + 1] * 5, bulletZ[i] - velocity[i * 3 + 2] * 5);


	}

	glEnd();
	Calculate();
	glLineWidth(1.0);
}

// NEED DISCUSSION
bool machineGun::CheckCollision(double cx, double cz, double r1, double r2, double h1, double h2)
{
	
	// bulletX[i] - velocity[i * 3] * 5, bulletY[i] - velocity[i * 3 + 1] * 5, bulletZ[i] - velocity[i * 3 + 2] * 5
	for (int s = 0 ; s < Number; s++) 
	{
		if (h1 < bulletY[s] && h1 + h2 > bulletY[s])
		{
			for (int i = 0; i < 10; i++)
			{
				double d2 = pow(fabs(bulletX[s] - velocity[s * 3] * 5 - cx), 2) + pow(fabs(bulletZ[s] - velocity[s * 3 + 2] * 5 - cz), 2);
				if (d2 < pow(r2, 2))
				{

					bulletX.erase(bulletX.begin() + s);
					bulletY.erase(bulletY.begin() + s);
					bulletZ.erase(bulletZ.begin() + s);

					velocity.erase(velocity.begin() + s * 3, velocity.begin() + s * 3 + 3);
					Number = Number - 1;
					
					return true;
				}
			}
		}

		if (h1 > bulletY[s] && bulletY[s]>0)
		{
			for (int i = 0; i < 10; i++)
			{
				double d2 = pow(fabs(bulletX[s] - velocity[s * 3] * 5 - cx), 2) + pow(fabs(bulletZ[s] - velocity[s * 3 + 2] * 5 - cz), 2);
				if (d2 < pow(r1, 2))
				{

					bulletX.erase(bulletX.begin() + s);
					bulletY.erase(bulletY.begin() + s);
					bulletZ.erase(bulletZ.begin() + s);

					velocity.erase(velocity.begin() + s * 3, velocity.begin() + s * 3 + 3);
					printf("xxxx:%d ", Number);
					Number = Number - 1;

					return true;
				}
			}
		}


	}

	return false;
	
}
