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
#include <ctime>
#include <unistd.h>

///////DEFINED TYPES////////////
typedef float Vec[3];
typedef float Flt;
///////////MACROS//////////////
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                                                (c)[1]=(a)[1]-(b)[1]; \
                                                (c)[2]=(a)[2]-(b)[2]
////////CONSTANTS//////////////
const float gravity = 0.2f;
using namespace std;
///////////FUNCTIONS/////////////
void renderBalloon();
void getPathcent(int arr[2]);
void getPatharr(unsigned char arr[18][18]);
void updateBlnPos();
void getStop(float x);
//void play();
///////////EXTERN////////////////
extern int getXres();
extern int getYres();
/////////////////////////////////

class Path
{
public:
	float cent[2];
	unsigned char mapArr[18][18];
	double startX;
	double startY;
	bool gotMap;
	Path() {
		gotMap = false;
	}
} pth;

class nGlobal
{
public:
	int xres;
	int yres;
	GLuint tttTexture;
        GLuint balloonTexture;
        int ttt;
        int balloon;
        nGlobal() {
                ttt=1;
                balloon=2;
        	xres = getXres();
        	yres = getYres();
		printf ("Yres: %d\n",yres);
	}
        ~nGlobal() {
        }

} nG;

class Balloon
{
public:
	
	Vec vel;
	float radius;
	double hDiff;
	double wDiff;
	float cent[2];
	float xCoor[4];
	float yCoor[4];
	int currX;
	int currY;
	int stop;
	int go;
	bool startNext;
	bool startMoving;
	struct Balloon* next;
	struct Balloon* prev;
	bool goleft;
	bool goright;
	bool goDown;
	bool goUp;
	Balloon() {
		next = NULL;
		prev = NULL;
		wDiff = 50;
		hDiff = 30.0;//37.5;
		go = 1;
		startMoving = false;
	}
} bln;

//IMAGE CLASS WAS NOT IN OUR CHOSEN FRAMEWORK, 
//HAD TO MIGRATE TO INDIVIVIDUAL CPP FILE
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


class nGameBln
{
public:
	Balloon *ahead;
	nGameBln() {
		srand(time(NULL));
		ahead = NULL;
       		for (int i = 0; i < 50; i++) {
        		Balloon *b = new Balloon;
                	b->radius = 25;
			
			///X COORDINATE
			b->cent[0] = pth.startY * 80;
       			printf ("B->CENT[0]: %f\n",b->cent[0]);
			b->currX = pth.startY -0.5;
			
			///Y COORDINATE
			b->cent[1] = pth.startX * 60 + nG.yres;
       			printf ("B->CENT[1]: %f\n",b->cent[1]);
			b->currY = pth.startX + 0.5;
			b->vel[0] = 0.05;
			b->vel[1] = 0.05;
			b->next = ahead;
                	if (ahead != NULL) {
                		ahead->prev = b;
                	}
                	ahead = b;
		}
	}
} nGame;

unsigned char *buildAlphaData(Image *img)
{
        int i;
        int a,b,c;
        unsigned char *newdata, *ptr;
        unsigned char *data = (unsigned char *)img->data;
        newdata = (unsigned char *)malloc(img->width * img->height * 4);
        ptr = newdata;
        for (i=0; i<img->width * img->height * 3; i+=3) {
                a = *(data+0);
                b = *(data+1);
                c = *(data+2);
                *(ptr+0) = a;
                *(ptr+1) = b;
                *(ptr+2) = c;
                *(ptr+3) = (a|b|c);
                ptr += 4;
                data += 3;
        }
        return newdata;
}

void initTTT()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
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
	glGenTextures(1, &nG.balloonTexture);
	glBindTexture(GL_TEXTURE_2D, nG.balloonTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *silhouetteData = buildAlphaData(&img[1]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[1].width, img[1].height, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
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


void getPatharr(unsigned char arr[18][18])
{
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (arr[i][j] == 's' || arr[i][j] == 'm') {
				if (arr[i][j] == 's') {
					pth.startX = -1*i - 0.5;
					////startY is actually horizantal position
					pth.startY = j + 0.5;
				}
				pth.mapArr[i][j] = arr[i][j];
			}
			if (arr[i][j] == 'b') {
				pth.mapArr[i][j] = arr[i][j];
			}
		}
	}
	pth.gotMap = true;
}
void updateBlnPos()
{
	Balloon *b = nGame.ahead;
		float dirx = 0.00;
		float diry = 0.00;
		int xx,yy;
		bool down = false;
		//bool left = false;
		//bool right = false;
	while(b) {
			/////////////////////chec if prev is null//////////
			if ( b->prev != NULL && b->prev->cent[1] > 1414 && b->prev->cent[0] > 600) {
				printf ("NOT NULL: %d\n",0);
				b->cent[0] += 0.00;
				b->cent[1] += 0.00;
			} else {
				b->startMoving = true;
			}
			
			if (b->prev == NULL || b->startMoving) {
			
			///////////////////////Down/////////////////////////
			if (pth.mapArr[b->currY+1][b->currX] == 'm') {
				b->goDown = true;
				b->goUp = false;
				if (b->goDown) {
					dirx = 0.00;
					diry = -1.00;
					if (b->go == 1) {
						b->go = 0;
						b->stop = b->cent[1] - 60;
						printf ("DOWN STOP COOR: %d\n",b->stop);
					}
				}
				/*
				if (b->go == 1) {
					b->go = 0;
					b->stop = b->cent[1] - 60;
					printf ("DOWN STOP COOR: %d\n",b->stop);
				}*/
			//down = true;
			}
			
			//////////////////////left////////////Right/////////
			else if (pth.mapArr[b->currY][b->currX-1] == 'm' || pth.mapArr[b->currY][b->currX+1] == 'm'){
				if (pth.mapArr[b->currY][b->currX+1] == 'b'){
					b->goleft = true;
					b->goright = false;
				}
				else if (pth.mapArr[b->currY][b->currX-1] == 'b'){
					b->goleft = false;
					b->goright = true;
				}

				if (b->goleft) {
					dirx = -2.00;
					diry = 0.00;
					if (b->go == 1) {
						b->go = 0;
						b->stop = b->cent[0] - 80;
					}
				}

				else if (b->goright) {
					dirx = 2.00;
					diry = 0.00;
					if (b->go == 1) {
						b->go = 0;
						b->stop = b->cent[0] + 80;
					}
				}
			}
			
			////////////////CHECKING///////////////////////////////////
			
			///////////////////DOWN///////////////////////////////////
			if ((b->cent[1] != b->stop) && b->goDown) {
				b->cent[0] += dirx;
				b->cent[1] += diry;
				printf ("DOWN POS: %f\n",b->cent[1]);
				printf ("DOWN moved COOR: %f\n",b->cent[1]);
			} else if (b->cent[1] == b->stop){
				if (b->goDown) {
					b->currX = b->currX;
					b->currY = b->currY+1;
				}
				b->goDown = false;
				b->go = 1;
				b->stop = 0;
				dirx = 0.00;
				diry = 0.00;
			}
			
			///////////////////LEFT or RIGHT////////////////////////
			if ((b->cent[0] != b->stop) && (b->goleft || b->goright)) {
				b->cent[0] += dirx;
				b->cent[1] += diry;
			} else if (b->cent[0] == b->stop) {
				if (b->goleft) {
					b->currX = b->currX - 1;
					b->currY = b->currY;
				}
				else if (b->goright) {
					b->currX = b->currX + 1;
					b->currY = b->currY;
				}
				//b->goleft = false;
				//b->goright = false;
				b->go = 1;
				b->stop = 0;
				dirx = 0.00;
				diry = 0.00;
			}
			}//if b->prev == null
			b=b->next;
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
void renderBalloon()
{
	Balloon *b = nGame.ahead;
	while (b) {
		glColor3f(1.0, 1.0, 1.0);
                if (nG.balloon) {
                        glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, nG.balloonTexture);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                        glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);
                                glVertex2i(b->cent[0] - b->wDiff ,b->cent[1] - b->hDiff );
                                glTexCoord2f(0.0f, 0.0f);
                                glVertex2i(b->cent[0] - b->wDiff ,b->cent[1] + b->hDiff );
                                glTexCoord2f(1.0f, 0.0f);
                                glVertex2i(b->cent[0] + b->wDiff ,b->cent[1] + b->hDiff );
                                glTexCoord2f(1.0f, 1.0f);
                                glVertex2i(b->cent[0] + b->wDiff ,b->cent[1] - b->hDiff );
                        glEnd();
                        glPopMatrix();
                        glDisable(GL_ALPHA_TEST);
			b = b->next;
                }
	}
}
