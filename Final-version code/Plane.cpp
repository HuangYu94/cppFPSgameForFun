#include "Plane.h"
#include "Missile.h"
#include <math.h>
int ParseString(
	int &nw, int wTop[], int wLen[],
	char str[],
	int maxNw)
{
	nw = 0;

	int state = 0;
	for (int i = 0; 0 != str[i]; ++i)
	{
		switch (state)
		{
		case 0:
			if (' ' != str[i] && '\t' != str[i] && ',' != str[i] && '\n' != str[i])
			{
				wTop[nw] = i;
				wLen[nw] = 1;
				++nw;
				state = 1;
			}
			break;
		case 1:
			if (' ' != str[i] && '\t' != str[i] && ',' != str[i] && '\n' != str[i])
			{
				++wLen[nw - 1];
			}
			else
			{
				state = 0;
				if (nw == maxNw)
				{
					goto LOOPOUT;
				}
			}
			break;
		}
	}
LOOPOUT:
	return nw;
}

char *MyFgets(char str[], int limit, FILE *fp)
{
	if (nullptr != fgets(str, 255, fp))
	{
		int l = 0;
		for (l = 0; 0 != str[l]; ++l)
		{
		}
		for (l = l - 1; 0 <= l; --l)
		{
			if ('\n' == str[l] || '\r' == str[l])
			{
				str[l] = 0;
			}
			else
			{
				break;
			}
		}
		return str;
	}
	else
	{
		return nullptr;
	}
}





Line_ ::Line_() {};
Line_::Line_(int xin1, int yin1, int xin2, int yin2, UINT8 rin, UINT8 gin, UINT8 bin) {
		x1 = xin1;
		y1 = yin1;
		x2 = xin2;
		y2 = yin2;
		r = rin;
		g = gin;
		b = bin;
	}
Line_::Line_(const Line_ &another) {
		x1 = another.x1;
		y1 = another.y1;
		x2 = another.x2;
		y2 = another.y2;
		r = another.r;
		g = another.g;
		b = another.b;
	}
Line_* Line_::operator=(const Line_ &another) {
		x1 = another.x1;
		y1 = another.y1;
		x2 = another.x2;
		y2 = another.y2;
		r = another.r;
		g = another.g;
		b = another.b;
		return this;
	}
void Line_:: draw(int width, int height, int shift_x, int shift_y) {
		glColor3ub(r, g, b);
		glBegin(GL_LINES);
		glVertex2i(x1 + shift_x, height - y1 + shift_y);
		glVertex2i(x2 + shift_x, height - y2 + shift_y);
		glEnd();
	}



Drawing_::Drawing_() {
		lines = nullptr;
		numberOflines = 0;
		cx = 0;
		cy = 0;
	}
Drawing_::Drawing_(const Drawing_ &d) {
		if (&d == this) {
			return;
		}
		else {
			if (lines != nullptr) {
				delete[] lines;
			}
			if (d.lines != nullptr && d.numberOflines >0) {
				lines = new Line_[d.numberOflines];
				numberOflines = d.numberOflines;
				for (int i = 0; i < d.numberOflines; i++) {
					lines[i] = d.lines[i];
				}
			}
		}
	}
Drawing_* Drawing_::operator=(const Drawing_ &d) {
		if (lines == d.lines) return this;
		if (lines != nullptr) {
			delete[] lines;
		}
		if (d.lines != nullptr && d.numberOflines >0) {
			lines = new Line_[d.numberOflines];
			numberOflines = d.numberOflines;
			for (int i = 0; i < d.numberOflines; i++) {
				lines[i] = d.lines[i];
			}
		}
		return this;
	}
bool Drawing_::Load(const char fn[]) {

		char str[256];
		FILE *fp = fopen(fn, "r");
		if (fp == nullptr) {
			return 0;
		}
		int lineNum = 0;
		bool fileNexist = true;
		while (nullptr != MyFgets(str, 255, fp)) {
			int nw = 0, wTop[16], wLen[16];
			fileNexist = false;
			if (ParseString(nw, wTop, wLen, str, 16) == 1) {
				numberOflines = atoi(str);
				lines = new Line_[numberOflines];
			}
			if (ParseString(nw, wTop, wLen, str, 16) == 7) {
				lines[lineNum] = Line_(atoi(str + wTop[0]), atoi(str + wTop[1]), atoi(str + wTop[2]),
					atoi(str + wTop[3]), atoi(str + wTop[4]), atoi(str + wTop[5]), atoi(str + wTop[6]));
				cx += (lines[lineNum].x1 + lines[lineNum].x2) / 2;
				cy += (lines[lineNum].y1 + lines[lineNum].y2) / 2;
				lineNum++;
			}
		}
		cx = cx / numberOflines;
		cy = cy / numberOflines;
		if (fileNexist) {
			return 0;
		}
		fclose(fp);
		return 1;
	}
void Drawing_::Scale(double scalar) {
		for (int i = 0; i < numberOflines; i++) {
			double relx1 = (double)(lines[i].x1 - cx) / scalar;
			lines[i].x1 = relx1 + cx;
			double relx2 = (double)(lines[i].x2 - cx) / scalar;
			lines[i].x2 = relx2 + cx;
			double rely1 = (double)(lines[i].y1 - cy) / scalar;
			lines[i].y1 = rely1 + cy;
			double rely2 = (double)(lines[i].y2 - cy) / scalar;
			lines[i].y2 = rely2 + cy;
		}
	}
void Drawing_::Draw(int width, int height, int shift_x, int shift_y) {
		for (int i = 0; i < numberOflines; i++) {
			lines[i].draw(width, height, shift_x, shift_y);
		}
	}
Drawing_::~Drawing_() {
		delete[] lines;
	}


Drawing_ LoadFile_(const char fn[])
{
	Drawing_ d;
	if (0 == d.Load(fn))
	{
		printf("Cannot read the file.\n");
		exit(1);
	}
	return d;
}
//utility functions and classes defination ends here

Vec3d::Vec3d(){};
Vec3d::Vec3d(double x, double y, double z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
};
Vec3d::Vec3d(const Vec3d &src) {
		for (int i = 0; i < 3; i++) {
			v[i] = src.v[i];
		}
}




Plane::Plane() {};
Plane::Plane(Vec3d c, double gLR, double mLR, int mCD, int numMiss, int gCD, Vec3d ypr_in,
		double col, double dam, double spe, double arm, const char fn[]) {
		ypr = ypr_in;
		center = c;
		numMissile = numMiss;
		gunLockRange = gLR;
		missileLockRange = mLR;
		gunCD = gCD;
		colisionBound = col;
		damage = dam;
		speed = spe;
		init_armor = arm;
		armor = arm;
		sketch = LoadFile_(fn);
		sketch.Scale(5.0);
	}
void Plane::DashLeft(double dashspeed,double dt) {
	Vec3d leftvec = Vec3d(heading.v[2], 0, -heading.v[0]);
	printf("%.4f\n", leftvec.v[0]);
	center = center + leftvec * dashspeed * dt;
}
void Plane::DashRight(double dashspeed, double dt) {
	Vec3d rightvec = Vec3d(-heading.v[2], 0, heading.v[0]);
	center = center + rightvec * dashspeed * dt;
}
void Plane::Move(double dt, Vec3d d_angle) {
		heading = cvtBodyVec(1.0);
		ypr = ypr + d_angle;
		center = center + heading * speed * dt;
	}
void Plane::onHit(double damage_in) {
		this->armor = this->armor - damage_in;
	}
void Plane::Render2D(int width, int height) {
		// draw gun lock range
		DrawCircle(width / 2, height / 2, 20);
		// draw missile lock range
		DrawRad((double)width / 2, (double)height / 2, -YsPi / 4.0, YsPi / 4.0, 300.0);
		DrawRad((double)width / 2, (double)height / 2, 3 * YsPi / 4.0, YsPi * 5 / 4.0, 300.0);
		sketch.Draw(width, height, 0, 0);
		// draw health
		if (armor > 0) {
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
			glVertex2d(130, height - 50);
			glVertex2d(130 + armor * 5, height - 50);
			glVertex2d(130 + armor * 5, height - 20);
			glVertex2d(130, height - 20);
			glEnd();
		}
}
Vec3d Plane::cvtBodyVec(double len) {
		Vec3d retVec(-len*cos(ypr.v[1])*sin(ypr.v[0]), len*sin(ypr.v[1]),
			-len*cos(ypr.v[1])*cos(ypr.v[0]));
		return retVec;
	}
Vec3d Plane::cvtSpanVec(double len) {
		Vec3d retVec(-len*cos(ypr.v[2])*sin(ypr.v[0]), len*sin(ypr.v[2]),
			-len*cos(ypr.v[2])*cos(ypr.v[0]));
		return retVec;
	}
void Plane::DrawCircle(int cx, int cy, int r)
	{
		glColor3ub(0, 255, 0);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i <= 64; ++i)
		{
			double angle = (double)i*YsPi / 32.0;
			double c = cos(angle);
			double s = sin(angle);
			glVertex2d((double)cx + c*r, (double)cy + s*r);
		}
		glEnd();
	}
void Plane::DrawRad(double ctrx, double ctry, double fromAng, double toAngle, double rad) {
		double step = (toAngle - fromAng) / 32.0;
		glColor3ub(255, 0, 0);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i <= 32; i++) {
			double angle = fromAng + i*step;
			double c = cos(angle);
			double s = sin(angle);
			glVertex2d(ctrx + c*rad, ctry + s*rad);
		}
		glEnd();
	}




//void DrawGroundLattice(void)
//{
//	glColor3f(0.0f, 0.0f, 1.0f);
//	glBegin(GL_LINES);
//	for (int x = -100; x <= 100; x += 20)
//	{
//		glVertex3i(x, 0, -100);
//		glVertex3i(x, 0, 100);
//
//		glVertex3i(-100, 0, x);
//		glVertex3i(100, 0, x);
//	}
//	glEnd();
//}

