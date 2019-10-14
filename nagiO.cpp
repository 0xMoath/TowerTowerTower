//Nagi Obeid Source File
//Four Steps of Debugging 10-09-2019
//1-Identify
//2-Isolate
//3-Fix
//4-Review
////////////////////////
#include <iostream>
#include <cstdlib>
#include <GL/glx.h>
#include "fonts.h"
#include <fstream>
using namespace std;

void showNagi(int x, int y)
{
    	Rect r;
    	r.center = 0;
	r.left = x;
	r.bot = y;
	ggprint8b(&r, 16, 0x00ff0000, "Nagi Obeid");
	ggprint8b(&r, 16, 0x00ff0000, "Credit Screen");
	ggprint8b(&r, 16, 0x00ff0000, "This is My stuff");
}
void printCredits(int xres, int yres)
{
	glClearColor(0.0,0.0,0.0,1.0);
	int xVal = xres / 2;
	int yVal = yres / 2;

	showNagi(xVal, yVal);
}
void clearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void checkScores(Rect r, int x, int y)
{
	ifstream fin("Names.txt");

	char name[3];
	int score[5];
	int hiscore = 0;
	int indx = 0;

	if (fin.is_open()) {
		while (fin >> name >> score[indx]) {
			indx++;
		}
		fin.close();
	}

	if (score[0] > score[1] && score[0] > score[2]) {
		hiscore = score[0];
	}
	else if (score[1] > score[2] && score[1] > score[0]) {
		hiscore = score[1];
	}
	else if (score[2] > score[0] && score[2] > score[1]) {
		hiscore = score[2];
	}
	
	r.bot = y/2;
	r.left = x/2;	
	ggprint16(&r, 16, 0xfff0, "HISCORE:%i", hiscore);

}
