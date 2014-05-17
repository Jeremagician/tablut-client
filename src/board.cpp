#include <iostream>
#include <SDL.h>
#include <GL/gl.h>

#include "board.hpp"

using namespace std;

tafl::board::board(int width, int height)
	: width_(width)
	, height_(height)
{

}

tafl::board::~board()
{

}

void tafl::board::render(void)
{
	glColor3ub(255, 100, 0);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glVertex3f(1,1,0);
	glVertex3f(0,1,0);
	glEnd();
}
