#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"
// utility functions and classes goes here
int ParseString(
	int &nw, int wTop[], int wLen[],
	char str[],
	int maxNw);

char *MyFgets(char str[], int limit, FILE *fp);



class Line_ {
public:
	int x1, y1, x2, y2;
	UINT8 r, g, b;

	Line_();
	Line_(int xin1, int yin1, int xin2, int yin2, UINT8 rin, UINT8 gin, UINT8 bin);
	Line_(const Line_ &another);
	Line_* operator=(const Line_ &another);
	void draw(int width, int height, int shift_x, int shift_y);
};

class Drawing_ {
private:
	Line_* lines = nullptr;
	int numberOflines;
	int cx, cy;
public:
	Drawing_();
	Drawing_(const Drawing_ &d);
	Drawing_* operator=(const Drawing_ &d);
	bool Load(const char fn[]);
	void Scale(double scalar);
	void Draw(int width, int height, int shift_x, int shift_y);
	~Drawing_();
};

Drawing_ LoadFile_(const char fn[]);
//utility functions and classes defination ends here

class Vec3d
{
public:
	double v[3];
	Vec3d();
	Vec3d(double x, double y, double z);
	Vec3d(const Vec3d &src);
	Vec3d Vec3d::operator=(Vec3d &another) {
		for (int i = 0; i < 3; i++) {
			v[i] = another.v[i];
		}
		return *this;
	}
	Vec3d Vec3d::operator+(Vec3d &another) {
		return Vec3d(v[0] + another.v[0], v[1] + another.v[1], v[2] + another.v[2]);
	}
	Vec3d Vec3d::operator-(Vec3d &another) {
		return Vec3d(v[0] - another.v[0], v[1] - another.v[1], v[2] - another.v[2]);
	}
	Vec3d Vec3d::operator*(double scalar) {
		return Vec3d(v[0] * scalar, v[1] * scalar, v[2] * scalar);
	}
	Vec3d Vec3d::operator/(double scalar) {
		return Vec3d(v[0] / scalar, v[1] / scalar, v[2] / scalar);
	}
};

class Plane {
public:
	Drawing_ sketch;
	Vec3d center;
	Vec3d heading;
	double gunLockRange;
	double missileLockRange;
	int missileCD;
	int gunCD;
	// yaw pitch roll
	Vec3d ypr;
	double colisionBound;
	double damage;
	double speed;
	double init_armor;
	double armor;
	int numMissile;
	Plane();
	Plane(Vec3d c, double gLR, double mLR, int mCD, int numMiss, int gCD, Vec3d ypr_in,
		double col, double dam, double spe, double arm, const char fn[]);
	void DashLeft(double dashspeed,double dt);
	void DashRight(double dashspeed,double dt);
	void Move(double dt, Vec3d d_angle);
	void onHit(double damage_in);
	void Render2D(int width, int height);
private:
	Vec3d cvtBodyVec(double len);
	Vec3d cvtSpanVec(double len);
	void DrawCircle(int cx, int cy, int r);
	void DrawRad(double ctrx, double ctry, double fromAng, double toAngle, double rad);
};

//void DrawGroundLattice(void);


