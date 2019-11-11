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
#include <cstring>
using namespace std;

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			sprintf(ppmname,"%s.ppm", name);
			char ts[100];
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			int n = width * height * 3;
			data = new unsigned char[n];
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			return;
	}
};
Image img[2] = {"./ttt.png", "./balloon.png"};
class GlobalImage {
public:
	GLuint tttTexture;
	GLuint balloonTexture;
	int ttt;
	int balloon;
	GlobalImage() {
		ttt=1;
		balloon=2;
	}
	~GlobalImage() {
	}
} nG;

void initTTT()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	//create opengl texture elements
	//////////////////////////////////
	//main img texture here
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &nG.tttTexture);
	glBindTexture(GL_TEXTURE_2D, nG.tttTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		img[0].width, img[0].height,
		0, GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	
	///////////////////////////////////////
	//ballon texture here
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &nG.balloonTexture);
	glBindTexture(GL_TEXTURE_2D, nG.balloonTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		img[1].width, img[1].height,
		0, GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
}

void renderTTT(int x, int y)
{
	glColor3f(1.0, 1.0, 1.0);
	if (nG.ttt) {
		glBindTexture(GL_TEXTURE_2D, nG.tttTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex2i(0, y);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex2i(x, y);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex2i(x, 0);
		glEnd();
	}
}

void renderBalloon(float x, float y)
{
	glColor3f(1.0, 1.0, 1.0);
	if (nG.balloon) {
		glBindTexture(GL_TEXTURE_2D, nG.balloonTexture);
		glBegin(GL_QUADS);
			/*
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);
			*/
			glTexCoord2f(0.0f, 1.0f); 
			glVertex2i(x,y);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex2i(x, y+y/2);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex2i(x+x/2, y+y/2);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex2i(x+x/2, y);
		glEnd();
	}
}

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

