#include "stlViewer.h"
#include "Plane.h"
#include "menu.h"

void STL::CleanUp(void)
{
	vtx.clear();
}
void STL::ReadFromFile(const char fn[])
{
	FILE *fp = fopen(fn, "r");
	if (nullptr != fp)
	{
		CleanUp();

		char str[256];
		while (nullptr != fgets(str, 255, fp))
		{
			StringParser parser;
			parser.Parse(str);

			if (4 <= parser.size())
			{
				char wd[256];
				parser.GetWord(wd, 0, 255);
				if (0 == strcmp(wd, "vertex"))
				{
					Vec3d v;
					parser.GetWord(wd, 1, 255);
					v.v[0] = atof(wd);
					parser.GetWord(wd, 2, 255);
					v.v[1] = atof(wd);
					parser.GetWord(wd, 3, 255);
					v.v[2] = atof(wd);
					vtx.push_back(v);
					//printf("%lf %lf %lf\n",v.v[0],v.v[1],v.v[2]);
				}
			}
		}

		fclose(fp);
	}
	else
	{
		printf("Cannot open file.\n");
	}
}

void STL::Draw(void) const
{
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i <= vtx.size() - 3; i += 3)
	{
		glVertex3dv(vtx[i].v);
		glVertex3dv(vtx[i + 1].v);
		glVertex3dv(vtx[i + 2].v);
	}
	glEnd();

	glColor3f(0, 0.5, 0.8);
	glBegin(GL_LINES);
	for (int i = 0; i <= vtx.size() - 3; i += 3)
	{
		glVertex3dv(vtx[i].v);
		glVertex3dv(vtx[i + 1].v);

		glVertex3dv(vtx[i + 1].v);
		glVertex3dv(vtx[i + 2].v);

		glVertex3dv(vtx[i + 2].v);
		glVertex3dv(vtx[i].v);
	}
	glEnd();
}


