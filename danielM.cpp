
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
//some stuff

void danielCredits(int x, int y)//,image??)
{
	Rect r;
	r.bot = y;
	r.left = x;
	r.center=0;

	ggprint16(&r, 16, 0xfff00fff, "DANIEL MCARTHUR");
	ggprint16(&r, 16, 0xffffffff, "something i did");
	ggprint16(&r, 16, 0xffffffff, "something else i did");
}

void printCredits(int xres, int yres)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//insert background
	int xCenter = xres/2;
	int yCenter = yres/2 ;
//	int spaceBetweenCredits = 42;
//	int yStart = 0;	//whatever the bottom of the page is
	//for(int i=0;i<99999; i++){//while not the exit event
		danielCredits(xCenter, yCenter);
		//showNagi(xCenter+42, yCenter+42);
		//MoathRend(xCenter+84, yCenter+84);
		//nagiCredits(ycenter,);
		//moathCredits(yStart-spaceBetweeenCredits + i,xcenter,image);
		//if(i>yres){i=0;}
	//};
}

