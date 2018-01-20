#include <math.h>
#include "fssimplewindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cannon.h"
#include <vector>
#include "Plane.h"

const double pi = 3.1415927;



void cMissile::initialize(void)
{
	x = 0;
	y = 0;
	z = 0;
	state = 0;
	alpha = 0;
	theta = 0;
	l = 5;
	damage = 10.0;
}

void cMissile::move(void)
{
	x = x + cos(theta)*cos(alpha)*v*0.01;
	y = y + sin(theta)*v*0.01;
	z = z + cos(theta)*sin(alpha)*v*0.01;
}


void cMissile::renderMissile(void)
{
	if (state != 0)
	{
		glColor3f(0.0, 0.2 , 1.0);
		glBegin(GL_QUADS);

		glVertex3d(x, y, z);
		glVertex3d(x + 1, y, z);
		glVertex3d(x + 1, y + 1, z);
		glVertex3d(x, y + 1, z);

		glVertex3d(x, y, z + 1);
		glVertex3d(x + 1, y, z + 1);
		glVertex3d(x + 1, y + 1, z + 1);
		glVertex3d(x, y + 1, z + 1);

		glVertex3d(x, y, z);
		glVertex3d(x + 1, y, z);
		glVertex3d(x + 1, y, z + 1);
		glVertex3d(x, y, z + 1);

		glVertex3d(x, y + 1, z);
		glVertex3d(x + 1, y + 1, z);
		glVertex3d(x + 1, y + 1, z + 1);
		glVertex3d(x, y + 1, z + 1);

		glVertex3d(x, y, z);
		glVertex3d(x, y + 1, z);
		glVertex3d(x, y + 1, z + 1);
		glVertex3d(x, y, z + 1);

		glVertex3d(x + 1, y, z);
		glVertex3d(x + 1, y + 1, z);
		glVertex3d(x + 1, y + 1, z + 1);
		glVertex3d(x + 1, y, z + 1);

		glEnd();
	}
}

std::vector<cMissile> cmis;
int ncMis = 0;


Cannon::Cannon()
{
	initialize();
	for (int i = 0; i < 10; i++)
	{
		x[i] = -500 + rand() % 1000;
		z[i] = -500 + rand() % 1000;

	}
}



void Cannon::initialize(void)
{
	r1 = 10;
	r2 = 5;
	h1 = 10.0;
	h2 = h1;
	cl = 10;
	
	srand((int)time(nullptr));
	
	for (int i = 0; i < 10; i++)
	{
		alpha[i] = 0.0;
		theta[i] = 0.0;
		health[i] = 205;
		state[i] = 1;
	}
	
}
bool Cannon::checkPlaneColision(Vec3d planeCtr, int cannonIdx) {
	double xp = planeCtr.v[0];
	double yp = planeCtr.v[1];
	double zp = planeCtr.v[2];
	if (sqrt(abs(xp - x[cannonIdx])*abs(xp - x[cannonIdx]) + abs(zp - z[cannonIdx])*abs(zp - z[cannonIdx]))
		<= r1 && yp <= (r1 + r2) && state[cannonIdx] != 0) {
		return true;
	}
	else {
		return false;
	}

}

void Cannon::getJetpos(double x, double y, double z)
{
	jx = x;
	jy = y;
	jz = z;
}


void Cannon::aim(void)
{
	for (int i = 0; i < 10; i++)
	{
		double dx = jx - x[i];
		double dz = jz - z[i];
		alpha[i] = atan2(dz, dx);
		double ds = sqrt(dx*dx + dz*dz);
		theta[i] = atan2((jy - (h1 + 0.5*h2)), ds);
	}
}

void Cannon::renderCannon(void) const
{
	for (int j = 0; j < 10; j++)
	{
		if (state[j] != 0 && health[j]>0)
		{
			glColor3ub(255 - health[j], 50, 50);
			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i<360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r1*c, h1, z[j] + r1*s);
			}
			glEnd();

			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i<360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r2*c, h1 + h2, z[j] + r2*s);
			}
			glEnd();

			glBegin(GL_QUAD_STRIP);
			for (int i = 0; i <= 360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r1*c, 0, z[j] + r1*s);
				glVertex3d(x[j] + r1*c, h1, z[j] + r1*s);
			}
			glEnd();

			glBegin(GL_QUAD_STRIP);
			for (int i = 0; i <= 360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r2*c, h1, z[j] + r2*s);
				glVertex3d(x[j] + r2*c, h1 + h2, z[j] + r2*s);
			}
			glEnd();

			glBegin(GL_LINES);
			glColor3d(0.0, 1.0, 0.0);
			glVertex3d(x[j], h1 + 0.5*h2, z[j]);
			glVertex3d(x[j] + cl*cos(theta[j])*cos(alpha[j]), h1 + 0.5*h2 + cl*sin(theta[j]), z[j] + cl*cos(theta[j])*sin(alpha[j]));
			glEnd();

			glBegin(GL_LINE_LOOP);
			for (int i = 0; i<360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r2*c, h1 + h2, z[j] + r2*s);
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			for (int i = 0; i<360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r2*c, h1, z[j] + r2*s);
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			for (int i = 0; i<360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r1*c, h1, z[j] + r1*s);
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			for (int i = 0; i<360; i += 30)
			{
				double a = (double)i* pi / 180.0;
				double c = cos(a);
				double s = sin(a);
				glVertex3d(x[j] + r1*c, 0, z[j] + r1*s);
			}
			glEnd();
		}
	}
	
}

void Cannon::reduceHealth(int i, bool hit)
{
	if (hit == true)
	{
		health[i] = health[i] - 1;
		if (health[i] <0)
		{
			health[i] = 0;

		}

	}

}

void Cannon::calculate_health(int index)
{
	health[index] -= 150;
	if (health[index] < 0)
	{
		health[index] = 0;
	}
}

void Cannon::set_state(int index)
{
	state[index] = 0;
}


bool cMissile::checkHitPlane(double px, double py, double pz, double pBounding) {
	if (sqrt((abs(px - x)*abs(px - x) + abs(py - y)*abs(py - y) + abs(pz - z))) < pBounding) {
		state = 0;
		return true;
	}
	return false;
}