//3350
//program: walk2.cpp
//author:  Gordon Griesel
//date:    summer 2017
//         spring 2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#include <fstream>
#include <iostream>

using namespace std;

//function prototypes
void MinitOpengl(void);
void Minit();
void Mphysics();
void Mrender();


class MImage;

int qsize;
class MGlobal {
    public:
        int grid_dim;
        int board_dim;
        MImage * ground;
        GLuint groundTex;
        MImage * icepath;
        GLuint iceTex;
        ~MGlobal() {
        }
        MGlobal() {
            grid_dim = 15;
            board_dim = 800;


        }
} mgl;
class MLevel {
    public:
        unsigned char arr[18][18];
        int nrows, ncols;
        MLevel() {
            //Log("Level constructor\n");
            //read level
            FILE *fpi = fopen("level1.txt","r");
            if (fpi) {
                nrows=0;
                char line[100];
                while (fgets(line, 100, fpi) != NULL) {
                    removeCrLf(line);
                    int slen = strlen(line);
                    ncols = slen;
                    //Log("line: %s\n", line);
                    for (int j=0; j<slen; j++) {
                        arr[nrows][j] = line[j];
                    }
                    ++nrows;
                }
                fclose(fpi);
                //printf("nrows of background data: %i\n", nrows);
            }
            for (int i=0; i<nrows; i++) {
                for (int j=0; j<ncols; j++) {
                    printf("%c", arr[i][j]);
                }
                printf("\n");
            }
        }
        void removeCrLf(char *str) {
            //remove carriage return and linefeed from a Cstring
            char *p = str;
            while (*p) {
                if (*p == 10 || *p == 13) {
                    *p = '\0';
                    break;
                }
                ++p;
            }
        }
} mlev;
class MImage {
    public:
        int width, height;
        unsigned char *data;
        ~MImage() { delete [] data; }
        MImage(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname);
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
                //printf("name **%s**\n", name);
                sprintf(ppmname,"%s.ppm", name);
                //printf("ppmname **%s**\n", ppmname);
                char ts[100];
                //system("convert eball.jpg eball.ppm");
                sprintf(ts, "convert %s %s", fname, ppmname);
                system(ts);
            }
            //sprintf(ts, "%s", name);
            //printf("read ppm **%s**\n", ppmname); fflush(stdout);
            FILE *fpi = fopen(ppmname, "r");
            if (fpi) {
                char line[200];
                fgets(line, 200, fpi);
                fgets(line, 200, fpi);
                //skip comments and blank lines
                while (line[0] == '#' || strlen(line) < 2)
                    fgets(line, 200, fpi);
                sscanf(line, "%i %i", &width, &height);
                fgets(line, 200, fpi);
                //get pixel data
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
                unlink(ppmname);
        }
};
MImage Mimg[2] = {
    "./images/dirt_full_new.png",
    "./images/ice_0_old.png"};



void MinitOpengl(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);

    glBindTexture(GL_TEXTURE_2D, 0);
    //load the images file into a ppm structure.
    int w = Mimg[0].width;
    int h = Mimg[0].height;
    mgl.ground = &Mimg[0];
    glGenTextures(1, &mgl.groundTex);
    //create opengl texture elements
    glBindTexture(GL_TEXTURE_2D, mgl.groundTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, mgl.ground->data);
    w = Mimg[1].width;
    h = Mimg[1].height;
    mgl.icepath = &Mimg[1];
    glGenTextures(1, &mgl.iceTex);
    //create opengl texture elements
    glBindTexture(GL_TEXTURE_2D, mgl.iceTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, mgl.icepath->data);
}
int qsize2;
void Minit() {
    qsize = 80;
    qsize2 = 60;
}


void get_grid_center(const int g, const int i, const int j, int cent[2])
{
    int s0 ;//= gl.xres;
    int s1 = qsize2;
    //This determines the center of each grid.
    switch(g) {
        case 1:
            s0 = qsize;
            break;
    }
    cent[0] = s0;
    cent[1] = s1;
    cent[0] *= j;
    cent[1] *= i;
}
void MoathRend(int x, int y, Rect r)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	x = x/2;
	y = y/2;
	
	r.bot =y;
	r.left=x;
	r.center=0;

	ggprint16(&r, 16, 0xfff0, "Moath's File");

}
void ShowScores(Rect r)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ifstream fin("Names.txt");

    char name[3];
    int score;
	if(fin.is_open()){ 
    while (fin >> name >> score)
    {
	ggprint8b(&r, 16, 0xfff0, name);
	ggprint8b(&r, 16, 0xfff0, "score:%i", score);
    
    }
    fin.close();}
}
void Mrender(int yres)
{
    //Rect r;
    int cent[2];
    
    for (int i=0; i<18; i++) {
        for (int j=0; j<18; j++) {
            if (mlev.arr[i][j] == 'm' || mlev.arr[i][j] == 'e' || mlev.arr[i][j] == 's') {
                glColor3f(1.0, 1.0, 1.0);
                //put tile in its place
                get_grid_center(1,i,j,cent);
                glBindTexture(GL_TEXTURE_2D, mgl.iceTex);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2i(cent[0],yres-cent[1]-qsize2);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2i(cent[0],yres-cent[1]);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2i(cent[0]+qsize,yres-cent[1]);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2i(cent[0]+qsize,yres-cent[1]-qsize2);
                glEnd();
            }
            if (mlev.arr[i][j] == 'b') {
                glColor3f(1.0, 1.0, 1.0);
                glBindTexture(GL_TEXTURE_2D, mgl.groundTex);
                get_grid_center(1,i,j,cent);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2i(cent[0],yres-cent[1]-qsize2);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2i(cent[0],yres-cent[1]);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2i(cent[0]+qsize,yres-cent[1]);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2i(cent[0]+qsize,yres-cent[1]-qsize2);
                glEnd();
            }
        }
    }
}
