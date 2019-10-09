//the four steps of debugging
//1. Recognize the bug
//2. Identify the cause of the bug
//3. Fix
//4. Test
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"


//Daniel McArthur
extern void showNagi(int x, int y);
extern void MoathRend(int x, int y, Rect r);
void danielCredits(int x, int y)
{
	Rect r;
	r.bot = y;
	r.left = x;
	r.center=0;

	ggprint16(&r, 16, 0xfff00fff, "DANIEL MCARTHUR");
	ggprint16(&r, 16, 0xffffffff, "something i did");
	ggprint16(&r, 16, 0xffffffff, "something else i did");
}

void doPrintCredits(int xres, int yres)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	int xCenter = xres/2;
	int yCenter = yres/2 ;
	danielCredits(xCenter, yCenter);
	showNagi(xCenter, yCenter+60);
	Rect r;
	MoathRend(xCenter, yCenter+84, r);
}

