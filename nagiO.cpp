//Nagi Obeid Source File
#include <iostream>
#include <cstdlib>
#include <GL/glx.h>
#include "fonts.h"
/*void showNagiPicture(int x, int y, float ty, GLuint textid);
{
    int wid = 40;
    float fx = 0.0f;
    static float angle = 0.0f;

    angle += 0.01;
    fx = sin(angle);
    y = y - 40;

    glPushMatrix();
    glTranslatef(x+(int)(fx*30.0),y,0);
    glcolor3f(1.0f, 0.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin(GL_QUADS);

}*/
void showNagi(int x, int y)
{
    	Rect r;
    	r.center = 0;
	r.left = x;
	r.bot = y;
	glClear(GL_COLOR_BUFFER_BIT);
	ggprint8b(&r, 16, 0x00ff0000, "Nagi Obeid");
	ggprint8b(&r, 16, 0x00ff0000, "Credit Screen");
	ggprint8b(&r, 16, 0x00ff0000, "This is My stuff");
}

