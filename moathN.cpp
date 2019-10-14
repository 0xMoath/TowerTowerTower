#include <iostream>

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "fonts.h"
#include <GL/glx.h>
#include <fstream>
 using namespace std;
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





