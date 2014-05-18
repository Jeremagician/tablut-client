#include <iostream>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "board.hpp"

using namespace std;

tafl::board::board(int width, int height)
	: width_(width)
	, height_(height)
{
	int i;

	cells_ = new cell*[height]; // Can throw but no memleak if it does
	try
	{
		for(i = 0; i < height; i++)
			cells_[i] = new cell[width];
	}
	catch(...)
	{
		/* If there is an allocation exception
		 * Free previous allocation and rethrow
		 */
		while(i --> 0)
			delete[] cells_[i];
		delete[] cells_;
		throw;
	}

	// King
	pawns_.push_back(pawn(4,4,king));

	// Swedish
	pawns_.push_back(pawn(5,4,swedish));
	pawns_.push_back(pawn(6,4,swedish));

	pawns_.push_back(pawn(3,4,swedish));
	pawns_.push_back(pawn(2,4,swedish));

	pawns_.push_back(pawn(4,5,swedish));
	pawns_.push_back(pawn(4,6,swedish));

	pawns_.push_back(pawn(4,3,swedish));
	pawns_.push_back(pawn(4,2,swedish));

	// Muscovites
	pawns_.push_back(pawn(3,0,muscovite));
	pawns_.push_back(pawn(4,0,muscovite));
	pawns_.push_back(pawn(4,1,muscovite));
	pawns_.push_back(pawn(5,0,muscovite));

	pawns_.push_back(pawn(0,3,muscovite));
	pawns_.push_back(pawn(0,4,muscovite));
	pawns_.push_back(pawn(1,4,muscovite));
	pawns_.push_back(pawn(0,5,muscovite));

	pawns_.push_back(pawn(8,3,muscovite));
	pawns_.push_back(pawn(8,4,muscovite));
	pawns_.push_back(pawn(7,4,muscovite));
	pawns_.push_back(pawn(8,5,muscovite));

	pawns_.push_back(pawn(3,8,muscovite));
	pawns_.push_back(pawn(4,8,muscovite));
	pawns_.push_back(pawn(4,7,muscovite));
	pawns_.push_back(pawn(5,8,muscovite));

}

tafl::board::~board()
{
	for(unsigned i = 0; i < height_; i++)
		delete[] cells_[i];
	delete cells_;
}

void tafl::board::highlight(int x, int y, float r, float g, float b)
{
	/* Silently ignore if wrong coordinates */
	if(x < 0 || x >= static_cast<int>(width_) || y < 0 || y >= static_cast<int>(height_))
		return;

	cells_[y][x].is_highlighted = true;
	cells_[y][x].r = r;
	cells_[y][x].g = g;
	cells_[y][x].b = b;

}

void tafl::board::clear_highlight(void)
{
	for(unsigned i = 0; i < height_; i++)
		for(unsigned j = 0; j < width_; j++)
			cells_[i][j].is_highlighted = false;
}

void tafl::board::draw_board(void)
{
	glColor3f(1.0,1.0,1.0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	board_tex.bind();
	/* Draw game area */
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);             glVertex3f(0,0,0);
	glTexCoord2f(width_,0);        glVertex3f(width_,0,0);
	glTexCoord2f(width_, height_); glVertex3f(width_,height_,0);
	glTexCoord2f(0, height_);      glVertex3f(0,height_,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void tafl::board::draw_highlight(int x, int y)
{
	glColor4f(cells_[y][x].r, cells_[y][x].g, cells_[y][x].b, 0.2);
	glBegin(GL_QUADS);
	glVertex3f(x,y,0.01);
	glVertex3f(x+1,y,0.01);
	glVertex3f(x+1,y+1,0.01);
	glVertex3f(x,y+1,0.01);
	glEnd();
}

void tafl::board::render(void)
{
	glTranslatef(width_/-2.0, height_/-2.0, 0);

	draw_board();
	/* Draw board tiles */
	for(unsigned i = 0; i < height_; i++)
		for(unsigned j = 0; j < width_; j++)
			if(cells_[i][j].is_highlighted)
				draw_highlight(j, i);

	for(pawn& p : pawns_)
		p.render();
}

void tafl::board::pawn::render(void)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	model.render();
	glPopMatrix();
}

tafl::cell tafl::board::get_cell(int window_x, int window_y)
{
	float window_z;
	GLdouble modelview[16];
	GLdouble projection[16];
	GLint viewport[4];
	tafl::cell c;
	GLdouble x, y, z;

	/* Ask OpenGL for modelview matrix, projection matrix and viewport */
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	/* glReadPixels except y as the location from the lower left corner
	 * so we have to invert this coordinate */
	window_y = viewport[3] - window_y;

	/* Read pixel depth from depth buffer */
	glReadPixels(window_x, window_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window_z);

	/* Unproject coordinates */
	gluUnProject(window_x, window_y, window_z, modelview, projection, viewport, &x, &y, &z);

	if(window_z == 1) // Invalid cell
	{
		c.x = -1;
		c.y = -1;
	}
	else
	{
		c.x = static_cast<int>(x);
		c.y = static_cast<int>(y);
	}
	c.set_bounds(width_, height_);
	return c;
}
