#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "fonts.h"
#include <GL/glx.h>

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
