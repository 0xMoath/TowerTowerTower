//Moath Nafjan
//CMPS3350

#include <iostream>

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "fonts.h"
#include <GL/glx.h>


/* Four steps of Debugging of Gordons understanding 
 *1- Reproduce the bug
 *2- locate the bad code
 *3- fix the code
 *4- test and fix the whole program if needed, since they might some errors 		caused after fixing the bug
 * */
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

