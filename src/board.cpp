#include <iostream>
#include <SDL.h>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>

#include "board.hpp"
#include "utils.hpp"

using namespace std;

#define OVER_POS 0.03

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
	pawns_.push_back(pawn(4,4,models_[KING]));

	// Swedish
	pawns_.push_back(pawn(5,4,models_[SWEDISH]));
	pawns_.push_back(pawn(6,4,models_[SWEDISH]));

	pawns_.push_back(pawn(3,4,models_[SWEDISH]));
	pawns_.push_back(pawn(2,4,models_[SWEDISH]));

	pawns_.push_back(pawn(4,5,models_[SWEDISH]));
	pawns_.push_back(pawn(4,6,models_[SWEDISH]));

	pawns_.push_back(pawn(4,3,models_[SWEDISH]));
	pawns_.push_back(pawn(4,2,models_[SWEDISH]));

	// Muscovites
	pawns_.push_back(pawn(3,0,models_[MUSCOVITE]));
	pawns_.push_back(pawn(4,0,models_[MUSCOVITE]));
	pawns_.push_back(pawn(4,1,models_[MUSCOVITE]));
	pawns_.push_back(pawn(5,0,models_[MUSCOVITE]));

	pawns_.push_back(pawn(0,3,models_[MUSCOVITE]));
	pawns_.push_back(pawn(0,4,models_[MUSCOVITE]));
	pawns_.push_back(pawn(1,4,models_[MUSCOVITE]));
	pawns_.push_back(pawn(0,5,models_[MUSCOVITE]));

	pawns_.push_back(pawn(8,3,models_[MUSCOVITE]));
	pawns_.push_back(pawn(8,4,models_[MUSCOVITE]));
	pawns_.push_back(pawn(7,4,models_[MUSCOVITE]));
	pawns_.push_back(pawn(8,5,models_[MUSCOVITE]));

	pawns_.push_back(pawn(3,8,models_[MUSCOVITE]));
	pawns_.push_back(pawn(4,8,models_[MUSCOVITE]));
	pawns_.push_back(pawn(4,7,models_[MUSCOVITE]));
	pawns_.push_back(pawn(5,8,models_[MUSCOVITE]));

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

void tafl::board::unhighlight(int x, int y)
{
/* Silently ignore if wrong coordinates */
	if(x < 0 || x >= static_cast<int>(width_) || y < 0 || y >= static_cast<int>(height_))
		return;

	cells_[y][x].is_highlighted = false;
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

	glColor3f(0.02,0.02,0.02); // Switch to black
	glBegin(GL_LINES);
	for(unsigned i = 0; i <= width_; i++)
	{
		glVertex3f(i, 0, OVER_POS);
		glVertex3f(i, height_, OVER_POS);
	}
	for(unsigned i = 0; i <= height_; i++)
	{
		glVertex3f(0, i, OVER_POS);
		glVertex3f(width_, i, OVER_POS);
	}
	glEnd();
}

void tafl::board::draw_highlight(int x, int y)
{
	glPushMatrix();
	glColor4f(cells_[y][x].r, cells_[y][x].g, cells_[y][x].b, 0.2);
	glBegin(GL_QUADS);
	glVertex3f(x,y,OVER_POS);
	glVertex3f(x+1,y,OVER_POS);
	glVertex3f(x+1,y+1,OVER_POS);
	glVertex3f(x,y+1,OVER_POS);
	glEnd();
	glPopMatrix();
}

bool tafl::board::pawn_at(int x, int y)
{
	for(pawn &p : pawns_)
		if(p.x == x && p.y == y)
			return true;
	return false;
}

void tafl::board::pawn_add(int x, int y, pawn_type type)
{
	if(!pawn_at(x,y))
		pawns_.push_back(pawn(x,y,models_[type]));
}

void tafl::board::pawn_remove(int x, int y)
{
	for(auto it = pawns_.begin(); it != pawns_.end(); it++)
		if(it->x == x && it->y == y)
		{
			pawns_.erase(it); // haha, erase it, you got it ? ...
			break;
		}
}


void tafl::board::pawn_move(int sx, int sy, int dx, int dy)
{
	if(!pawn_at(dx, dy) && (dx != sx || dy != sy))
	{
		for(pawn &p : pawns_)
			if(p.x == sx && p.y == sy)
				p.move_to(dx, dy);
	}
}

void tafl::board::pawn::move_to(int tx, int ty)
{
	start_x = x;
	start_y = y;
	x = tx;
	y = ty;
	percent = 0;
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

	/* We save matrix states
	 * This will be used later if the game request an unproject */
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_);
	glGetDoublev(GL_PROJECTION_MATRIX, projection_);
	glGetIntegerv(GL_VIEWPORT, viewport_);
}

void tafl::board::update(uint32_t diff)
{
    for(pawn &p : pawns_)
	    p.update(diff);
}

void tafl::board::pawn::update(uint32_t diff)
{
	float amount = (diff / 1000.0) * 2;
	if(x != real_x || y != real_y)
	{
		// if pawn is close enough of target pos, stop the animation
		if( abs(x - real_x) < 0.1 && abs(y - real_y) < 0.1)
		{
			real_x = x;
			real_y = y;
		}
		else
		{
			percent+=amount;
			real_x = utils::serp(start_x, static_cast<float>(x), percent);
			real_y = utils::serp(start_y, static_cast<float>(y), percent);
		}
	}
}

void tafl::board::pawn::render(void)
{
	if(model != nullptr)
	{
		glPushMatrix();
		glTranslatef(real_x, real_y, 0.02);
		model->render();
		glPopMatrix();
	}
}

tafl::board::pawn:: pawn(int x, int y, pawn_model *model)
	: x(x), y(y)
	, real_x(x), real_y(y)
	, percent(0)
	, model(model)
{
}

tafl::cell tafl::board::get_cell(int window_x, int window_y)
{
	float window_z;
	tafl::cell c;
	GLdouble x, y, z;

	/* Ask OpenGL for modelview matrix, projection matrix and viewport */


	/* glReadPixels except y as the location from the lower left corner
	 * so we have to invert this coordinate */
	window_y = viewport_[3] - window_y;

	/* Read pixel depth from depth buffer */
	glReadPixels(window_x, window_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window_z);

	/* Unproject coordinates */
	gluUnProject(window_x, window_y, window_z, modelview_, projection_, viewport_, &x, &y, &z);

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
