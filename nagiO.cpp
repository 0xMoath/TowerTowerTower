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
/*
void getPathcent(int arr[2])
{
	pth.cent[0] = arr[0];
	pth.cent[1] = arr[1];
       	printf ("cent[0]: %d\n",pth.cent[0]);
       	printf ("cent[1]: %d\n",pth.cent[1]);
}
*/
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
	double cent[2];
	float xCoor[4];
	float yCoor[4];
	struct Balloon* next;
	struct Balloon* prev;
	Balloon() {
		next = NULL;
		prev = NULL;
		wDiff = 50;
		hDiff = 37.5;
	}
} bln;

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
       		for (int i = 0; i < 1; i++) {
        		Balloon *b = new Balloon;
                	b->radius = 25;
			//b->cent[0] = (rand() % 18);
			//b->cent[1] = (rand() % 18);
			b->cent[0] = pth.startY * 80;
			b->cent[1] = pth.startX * 60 + nG.yres;
       		//	printf ("STARTx: %d\n",pth.startX);
       		//	printf ("STARTy: %d\n",pth.startY);
          		
			b->xCoor[0] = b->cent[0] - b->wDiff;
			b->yCoor[0] = b->cent[1] - b->hDiff;
			
          		b->xCoor[1] = b->cent[0] - b->wDiff;
			b->yCoor[1] = b->cent[1] + b->hDiff;
			
			b->xCoor[2] = b->cent[0] + b->wDiff;
			b->yCoor[2] = b->cent[1] + b->hDiff;

          		b->xCoor[3] = b->cent[0] + b->wDiff;
			b->yCoor[3] = b->cent[1] - b->hDiff;
			
			//b->vel[0] = (Flt)(rnd()*2.0-1.0);
			//b->vel[1] = (Flt)(rnd()*2.0-1.0);
			b->vel[0] = (0.05*2.0-1.0);
			b->vel[1] = (0.05*2.0-1.0);
			printf ("B-VEL[0]: %f\n",b->vel[0]);
			printf ("B-VEL[1]: %f\n",b->vel[1]);
			b->next = ahead;
                	if (ahead != NULL) {
                		ahead->prev = b;
                	}
                	ahead = b;
		}
		//printf ("AHEAD CLASS: %p\n",ahead);
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
					pth.startY = j + 0.5;
       				//	printf ("STARTx: %d\n",pth.startX);
       				//	printf ("STARTy: %d\n",pth.startY);
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
	float dirx = 0;
	float diry = 0;
	Balloon *b = nGame.ahead;
	//printf ("AHEAD from UPDATE: %p\n",nGame.ahead);	
	while(b) {
		for (int i = 0; i < 18; i++) {
			for (int j = 0; j < 18; j++) {
					////////////////////starting pos
					if (pth.mapArr[i][j] == 's' || pth.mapArr[i][j] == 'm') {
						dirx = 0;
						diry = 0;
					//b->cent[0] = (j-0.5) * 85;
					//b->cent[1] = -1 * (i+0.5) * 60 + nG.yres;
					}
					//////////////Dm-Lb-Rb
					if (pth.mapArr[i-1][j] == 'b' && pth.mapArr[i+1][j] == 'b'
							&& pth.mapArr[i][j+1] == 'm') {
						dirx = 0;
						diry = -1;
					}
					//////////////Usm-Db-Lm-Rb
					else if (pth.mapArr[i+1][j] == 'b' && pth.mapArr[i][j+1] == 'b'
							&& pth.mapArr[i-1][j] == 'm'
							&& (pth.mapArr[i][j-1] == 'm'
							|| pth.mapArr[i][j-1] == 's')) {
						dirx = -1;
						diry = 0;
					}
					//////////////Ub-Db-Lm-Rm
					else if (pth.mapArr[i][j-1] == 'b' && pth.mapArr[i][j+1] == 'b'
							&& pth.mapArr[i-1][j] == 'm'
							&& pth.mapArr[i+1][j] == 'm') {
						dirx = -1;
						diry = 0;
					}
					//////////////Ub-Dm-Lb-Rm
					else if (pth.mapArr[i-1][j] == 'b' && pth.mapArr[i][j-1] == 'b'
							&& pth.mapArr[i+1][j] == 'm'
							&& pth.mapArr[i][j+1] == 'm') {
					
					}
					b->cent[0] += b->vel[0]*dirx /20;
					b->cent[1] += b->vel[1]*diry /20;
					
					b->xCoor[0] = b->cent[0] - b->wDiff;
					b->yCoor[0] = b->cent[1] - b->hDiff;
			
          				b->xCoor[1] = b->cent[0] - b->wDiff;
					b->yCoor[1] = b->cent[1] + b->hDiff;
			
					b->xCoor[2] = b->cent[0] + b->wDiff;
					b->yCoor[2] = b->cent[1] + b->hDiff;

          				b->xCoor[3] = b->cent[0] + b->wDiff;
					b->yCoor[3] = b->cent[1] - b->hDiff;
					//printf ("MAPARR: %c\n",pth.mapArr[i][j]);
			}
		}
		b = b->next;
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
	//printf ("AHEAD from RENDERBALLOON: %p\n",nGame.ahead);
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
                                glVertex2i(b->xCoor[0],b->yCoor[0]);
                                glTexCoord2f(0.0f, 0.0f);
                                glVertex2i(b->xCoor[1], b->yCoor[1]);
                                glTexCoord2f(1.0f, 0.0f);
                                glVertex2i(b->xCoor[2], b->yCoor[2]);
                                glTexCoord2f(1.0f, 1.0f);
                                glVertex2i(b->xCoor[3], b->yCoor[3]);
                        glEnd();
                        glPopMatrix();
                        glDisable(GL_ALPHA_TEST);
			b = b->next;
                }
	}
}
