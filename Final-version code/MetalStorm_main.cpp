#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Missile.h"
#include "cannon.h"
#include "menu.h"
#include "machineGun.h"
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include "camera.h"
#include "Plane.h"
#include "Map.h"
//#include "yspng.h"

// const double YsPi = 3.1415927;

// utility functions and classes goes here

const double h_min = -1000.0;
const double h_max = 1000.0;
const double v_min = 0.0;
const double v_max = 500.0;

int main(void)
{
	int terminate = 0;
	bool gameover = false;
	CameraObject camera,camera_menu;
	// initial values for plane position
	Vec3d init_c(300, 300, -20), init_pos(0, 0, 0);
	//Plane fighterJet = Plane(init_c, 50., 30., 5, 20, 20,init_pos, 4., 0., 10., 1., test_fn);

	camera.z = 10.0;
	camera.y = 5.0;

	// if mouse position lie within a margin, we dont move
	int lb, mb, rb, mx, my;
	int boundingR, boundingx, boundingy;
	boundingx = 800 / 2;
	boundingy = 600 / 2;
	boundingR = 10;
	// end defination of mouse values
	// int terminate = 0;
	
	// def menu
	int k = 0;
	menu menu;
	int menuModel = 1;

	FsOpenWindow(16, 16, 800, 600, 1);

	// def audio
	YsSoundPlayer player1;
	YsSoundPlayer::SoundData bgm;
	YsSoundPlayer player2;
	YsSoundPlayer::SoundData gunaudio;
	YsSoundPlayer player3;
	YsSoundPlayer::SoundData missileaudio;
	player1.Start();
	player2.Start();
	player3.Start();
	missileaudio.LoadWav("Missile.wav");
	gunaudio.LoadWav("gun.wav");

	FsPassedTime();
	while (0 == terminate)
	{
		if (menuModel == 1)
			goto MENU;
		else
			goto GAME;

	MENU:
		double dist = 10.0;
		bgm.LoadWav("menu.wav");
		player1.PlayBackground(bgm);
		for (;;)
		{
			FsPollDevice();
			auto key = FsInkey();
			if (0 != FsGetKeyState(FSKEY_Z))
			{
				dist /= 1.1;
				if (dist<0.1)
				{
					dist = 0.1;
				}
			}
			if (0 != FsGetKeyState(FSKEY_M))
			{
				dist *= 1.1;
			}
			if (FSKEY_RIGHT == key || FSKEY_LEFT == key)
			{
				k = k + 1;
				k = k % 2;
			}
			if (FSKEY_ESC == key)
			{
				terminate = 1;
				goto FINAL;
			}
			if (FSKEY_ENTER == key)
			{
				bgm.LoadWav("game.wav");
				menuModel = 0;
				break;
			}
			double dt = (double)FsPassedTime() / 1000.0;
			camera_menu.h += dt  *YsPi/12;
			camera_menu.p = -YsPi / 5;
			int wid, hei;
			FsGetWindowSize(wid, hei);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			glViewport(0, 0, wid, hei);

			// Set up 3D drawing
			double vx, vy, vz;
			camera_menu.GetForwardVector(vx, vy, vz);
			camera_menu.x = 0.0 - vx*dist;
			camera_menu.y = 0.0 - vy*dist;
			camera_menu.z = 0.0 - vz*dist;

			camera_menu.SetUpCameraProjection();
			camera_menu.SetUpCameraTransformation();

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1, 1);
			// 3D drawing from here
			ability temp_a;
			temp_a = menu.runDraw(k);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -20, 20);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glDisable(GL_DEPTH_TEST);

			// 2D drawing from here
			temp_a.drawAbility();

			FsSwapBuffers();
			FsSleep(10);

		}
		//printf("while %d\n", menuModel);

	GAME:
		int i = 0;
		double v = 30;
		const int nM = 20;
		const int t_reload = 5;
		auto t0 = -t_reload;
		Missile missile[nM];
		std::vector<cMissile> cmis;
		int ncMis = 0;
		int loopcount = 0;

		// define missile values
		int nlive = nM;
		int mflag = 0;
		// cannon and map def
		Cannon can;

		// machine gun
		machineGun gun;

		// initialize machine gun parameter
		gun.setwh();

		const char* test_fn = menu.returnModelName(k);
		Plane fighterJet = Plane(init_c, 50., 30., 5, 20, 20, init_pos, 10., 0., 10., 100., test_fn);
		// define map variables
		GLuint texId = ReadImage("ground.png");
		GLuint texId1 = ReadImage("sky.png");
		GLuint texId2 = ReadImage("sky.png");
		GLuint texId3 = ReadImage("sky.png");
		GLuint texId4 = ReadImage("sky.png");
		GLuint texId5 = ReadImage("sky.png");
		can.initialize();

		// play bgm
		player1.PlayBackground(bgm);

		for (;;) {
			FsPollDevice();

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			//printf("%d", menuModel);
			double dt = (double)FsPassedTime() / 1000.0;

			int wid, hei;
			FsGetWindowSize(wid, hei);

			int key = FsInkey();

			switch (key)
			{
			case FSKEY_ESC:
				terminate = 1;
				goto FINAL;
			}

			FsGetMouseEvent(lb, mb, rb, mx, my);
			// check whether mouse go out of bound
			double head_diff = 0, pitch_diff = 0;
			if (sqrt((mx - boundingx)*(mx - boundingx) + (my - boundingy)*(my - boundingy)) > boundingR) {
				head_diff = -(double)(mx - wid / 2) / wid * 2 * YsPi / 6 * dt;
				pitch_diff = -(double)(my - hei / 2) / hei * 2 * YsPi / 6 * dt;
				//camera.h += -(double)(mx - 400) / 400 * YsPi / 6 * dt;
				//camera.p += -(double)(my - 400) / 400 * YsPi / 6 * dt;
				//double vx, vy, vz;
				//camera.GetForwardVector(vx, vy, vz);
				//camera.x += vx*dt*5.0;
				//camera.y += vy*dt*5.0;
				//camera.z += vz*dt*5.0;
			}
			// accelerate
			//FsPollDevice();
			//int key1 = FsInkey();




			// controller process begins here 
			// move fighter jet
			fighterJet.Move(dt, Vec3d(head_diff, pitch_diff, 0));
			if (FSKEY_W == key && fighterJet.speed < 50) {
				fighterJet.speed += 1;
			}
			if (FSKEY_S == key && fighterJet.speed > 1.) {
				fighterJet.speed -= 1;
			}
			if (FSKEY_A == key) {
				fighterJet.DashLeft(80., dt);
			}
			if (FSKEY_D == key) {
				fighterJet.DashRight(80., dt);
			}
			// cannon auto firing
			can.getJetpos(camera.x, camera.y, camera.z);
			can.aim();
			loopcount++;
			int nCan = 0;
			if (loopcount % 100 == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					nCan += can.state[i];

					if (can.state[i] == 1)
					{
						ncMis++;
						cmis.resize(ncMis);
						cmis[ncMis - 1].x = can.x[i];
						cmis[ncMis - 1].z = can.z[i];
						cmis[ncMis - 1].y = can.h1 + 0.5*can.h2;
						cmis[ncMis - 1].alpha = can.alpha[i];
						cmis[ncMis - 1].theta = can.theta[i];
						cmis[ncMis - 1].state = 1;
						cmis[ncMis - 1].v = 100;
						//printf("%d shoot! \n", ncMis);


					}
				}
				auto t2 = time(NULL);
				if (0 == nCan)
				{
					for (;;)
					{
						auto t3 = time(NULL);
						if (t3 - t2 > 3)
						{
							char str[80];
							strcpy(str, "YOU WIN!!!");
							glColor3ub(255, 0, 0);
							glRasterPos2i(32 * 10, 32 * 3);
							YsGlDrawFontBitmap12x16(str);
							Sleep(3000);
						}
						goto MENU;
					}
				}
			}

			int nerase = 0;
			for (int i = 0; i < ncMis; i++)
			{
				if (cmis[i].y > 500 || cmis[i].x > 1000 || cmis[i].x < -1000 || cmis[i].z > 1000 || cmis[i].z < -1000 || cmis[i].state == 0)
				{
					cmis.erase(cmis.begin() + i - nerase);
					nerase++;
					ncMis--;
				}
				cmis[i].move();
				//printf("%f %f %f\n", cmis[1].x, cmis[1].y, cmis[1].z);
				if (true==cmis[i].checkHitPlane(fighterJet.center.v[0], fighterJet.center.v[1], 
					fighterJet.center.v[2], fighterJet.colisionBound)) {
					fighterJet.onHit(3);
					//fighterJet.armor -= 5;
					printf("health: %.4f\n", fighterJet.armor);

					if (fighterJet.armor < 0) {
						/*auto t4 = time(NULL);
						for (;;)
						{
							auto t5 = time(NULL);
							if (t5 - t4 < 3)
							{
								char str[80];
								strcpy(str, "YOU DIED!!! GAME OVER!!!");
								glColor3ub(255, 0, 0);
								glRasterPos2i(32 * 15, 32 * 2);
								YsGlDrawFontBitmap12x16(str);
								Sleep(3000);
							}
							else
							{
								goto MENU;
							}
						}*/
						char str[80];
						strcpy(str, "YOU DIED!!! GAME OVER!!!");
						glColor3ub(255, 0, 0);
						glRasterPos2i(32 * 10, 32 * 3);
						YsGlDrawFontBitmap12x16(str);
						Sleep(3000);
						goto MENU;
					}
				}

			}

			auto t1 = time(NULL);
			auto tpass = t1 - t0;

			if (i < nM && tpass >= t_reload)
			{
				mflag = 0;
			}

			// set missile velocity
			
			if (key==FSKEY_F) {

				auto t1 = time(NULL);
				auto tpass = t1 - t0;
				//printf("detected mouse! %d", tpass);
				if (i % 2 == 0 && i < nM && i != 0)
				{
					if (tpass >= t_reload)
					{
						player3.PlayOneShot(missileaudio);

						missile[i].x = fighterJet.center.v[0];
						missile[i].y = fighterJet.center.v[1];
						missile[i].z = fighterJet.center.v[2];
						missile[i].setState(1);
						//printf("setting initial speed");
						missile[i].setV(fighterJet.heading.v[0], fighterJet.heading.v[1], fighterJet.heading.v[2], v);
						i++;
						nlive--;
						mflag = 0;
					}
					else
					{
						//printf("Reloading!!!\n");
						mflag = 1;
					}
				}
				else if (i >= nM)
				{
					//printf("No Missile!!!\n");
					mflag = 2;
				}
				else
				{
					player3.PlayOneShot(missileaudio);

					missile[i].x = fighterJet.center.v[0];
					missile[i].y = fighterJet.center.v[1];
					missile[i].z = fighterJet.center.v[2];
					missile[i].setState(1);
					missile[i].setV(fighterJet.heading.v[0], fighterJet.heading.v[1], fighterJet.heading.v[2], v);
					i++;
					nlive--;
					t0 = time(NULL);
				}
			}
			// viewer process begins here
			camera.h = fighterJet.ypr.v[0];
			camera.p = fighterJet.ypr.v[1];
			camera.x = fighterJet.center.v[0];
			camera.y = fighterJet.center.v[1];
			camera.z = fighterJet.center.v[2];
			//printf("camera position %1.4f %1.4f %1.4f\n", camera.x, camera.y, camera.z);
			if (FSKEY_B == key) {
				menuModel = 1;
				break;
			}

			// set machine gun shooting position	
			if (lb)
			{
				
				//gunaudio.LoadWav("gun.wav");
				player2.PlayOneShot(gunaudio);
				gun.setPosition(camera.x, camera.y, camera.z, fighterJet.heading.v[0], fighterJet.heading.v[1], fighterJet.heading.v[2]); //set where I am (head xyz) and velocity vector;
				//printf("cameraz  %1.4f  ", fighterJet.heading.v[2]);
				//player2.End();
				
			}

			//printf("break out menu...\n");
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			glViewport(0, 0, wid, hei);

			// Set up 3D drawing
			camera.SetUpCameraProjection();
			camera.SetUpCameraTransformation();

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1, 1);

			// 3D drawing from here
			//printf("drawing 3d\n");
			//DrawGroundLattice();
			// draw map
			

			//can.initialize();
			can.renderCannon();
			for (int i = 0; i < ncMis; i++)
			{
				cmis[i].renderMissile();

			}
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glColor4d(1.0, 1.0, 1.0, 1.0);

			PrintImagexz(texId, h_min, h_max, h_min, h_max, v_min + 5, v_min + 5);
			PrintImagexz(texId1, h_min, h_max, h_min, h_max, v_max - 5, v_max - 5);
			PrintImageyz(texId2, h_min + 5, h_min + 5, h_min, h_max, v_min, v_max);
			PrintImageyz(texId3, h_max - 5, h_max - 5, h_min, h_max, v_min, v_max);
			PrintImagexy(texId4, h_min, h_max, h_min + 5, h_min + 5, v_min, v_max);
			PrintImagexy(texId5, h_min, h_max, h_max - 5, h_max - 5, v_min, v_max);

			glDisable(GL_TEXTURE);
			
			for (int i = 0; i<10; i++)
			{
				if (can.health[i]>0)
				{
					can.reduceHealth(i, gun.CheckCollision(can.x[i], can.z[i], can.r1, can.r2, can.h1, can.h2));
				}
				if (0 >= can.health[i])
				{
					can.set_state(i);
				}
				if (can.checkPlaneColision(fighterJet.center, i) || fighterJet.center.v[0] < -1000||
					fighterJet.center.v[0]>1000||fighterJet.center.v[1]<0||fighterJet.center.v[1]>500||
					fighterJet.center.v[2]<-1000||fighterJet.center.v[2]>1000) {
					char str[80];
					strcpy(str, "You Clashed into Something!! Game Over!!");
					glColor3ub(255, 0, 0);
					glRasterPos2i(32 * 10, 32 * 3);
					YsGlDrawFontBitmap12x16(str);
					Sleep(3000);
					goto MENU;
				}
			}


			for (int m = 0; m < nM; m++)
			{
				if (1 == missile[m].getState() && 0.0 <= missile[m].getY())
				{
					missile[m].Move(dt);
					for (int i = 0; i < 10; i++)
					{
						if (0 < can.health[i] && missile[m].CheckCollision(can.x[i], can.z[i], can.r1, can.r2, can.h1, can.h2))
						{
							can.calculate_health(i);
							if (0 >= can.health[i])
							{
								can.set_state(i);
							}
							missile[m].setState(0);
							break;
						}
					}
					if (1 == missile[m].getState())
					{
						missile[m].Draw();
					}

				}
			}
			//draw machine gun bullets
			gun.Gun();
			//fighterJet.Render3D();

			// Set up 2D drawing
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glDisable(GL_DEPTH_TEST);

			// 2D drawing from here
			fighterJet.Render2D(wid, hei);
			gun.drawHeat();
			Draw_nMissile(nlive);
			if (1 == mflag)
			{
				Draw_ReLoad();
			}
			else if (2 == mflag)
			{
				Draw_NoMissile();
			}
			else if (0 == mflag)
			{
				Draw_Ready();
			}
			//player2.End();
			//player2.Start();

			FsSwapBuffers();
			FsSleep(10);
		}
	

	}
FINAL:
	return 0;
}