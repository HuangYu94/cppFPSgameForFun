#pragma once
#include <vector>
#include "Plane.h"
#include "Missile.h"

class STL
{
public:
	std::vector <Vec3d> vtx;

	void CleanUp(void);
	void ReadFromFile(const char fn[]);
	void Draw(void) const;
};



