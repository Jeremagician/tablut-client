#include <iostream>
#include <list>
#include <exception>
#include <stdexcept>
#include <string>
#include <GL/gl.h>

#include "game.hpp"
#include "SDL.hpp"
#include "window.hpp"
#include "board.hpp"
#include "texture.hpp"
#include "utils.hpp"

using namespace std;

tafl::game::game(int argc, char** argv)
	: moving_camera_(false)
	, speed_(-1)
	, max_framerate_(80)
{
	list<string> args;
	int tw = 9, th = 9;
	for(int i = 1; i < argc; i++)
		args.push_back(argv[i]);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window_ = new window("Tablut", 800, 600); // Can throw
	board_ = new tafl::board(tw,th); // Can trow aswell but safe
	camera_ = new tafl::centered_camera(vec3(0,0,0), utils::max(tw, th),
										utils::deg_to_rad(65), 0,
										vec3(0,0,1),
										60,
										static_cast<float>(window_->width())/window_->height());

/*	camera_ = new tafl::camera(vec3(0,-tw/3.0, utils::max(tw, th)),
							   vec3(0,0,0),
							   vec3(0,0,1),
							   60,
							   static_cast<float>(window_->width())/window_->height()
							   );*/
	font_ = new tafl::font("data/fonts/uncadis.ttf", 22.0);

	init_gl();

	if(!net.open("localhost", "31415"))
	{
		cerr << "Can't open network" << endl;
	}
}

tafl::game::~game()
{
	delete font_;
	delete camera_;
	delete board_;
	delete window_;
}

void tafl::game::init_gl(void)
{
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);


	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat ambient[] = {1.f, 1.f, 1.f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

int tafl::game::operator()()
{
	unsigned last_time, current_time, diff, frame_time, sleep_time;
	unsigned ms_per_frame = 1000.0/max_framerate_;
	unsigned frames = 0, speed_time = 0;

	speed_time = last_time = SDL_GetTicks();
	while(running_)
	{
		current_time = SDL_GetTicks();
		diff = current_time - last_time;

		input();
		update(diff);
		render();

		last_time = current_time;
		/* Sleep if we are rendering too much */
		frame_time = SDL_GetTicks() - last_time;
		if(frame_time < ms_per_frame)
		{
			sleep_time = ms_per_frame - frame_time;
			SDL_Delay(sleep_time);
		}
		else
			sleep_time = 0;

		frames++;
		if(SDL_GetTicks() - speed_time > 250)
		{
			speed_time = SDL_GetTicks();
			speed_ = 1000.0/(frames*4);
			frames = 0;
		}
	}

	return EXIT_SUCCESS;
}

void tafl::game::update(uint32_t diff)
{
	board_->update(diff);
}

void tafl::game::render(void)
{
	glClearColor(0,0.4,0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	camera_->render();
	board_->render();
	glPopMatrix();

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_->width(), window_->height(), 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	if(speed_ != -1)
		font_->render(10,0, to_string(static_cast<int>(1000.0/speed_)) + string(" fps"));
	font_->render(window_->width()*0.82,0,"Tafl - alpha 0.1");
	glPopMatrix();

	window_->swap();
}

void tafl::game::highlight_possible_moves(cell &c)
{
	if(!c.is_valid())
		return;
	cell temp(c);
	board_->highlight(temp.x, temp.y, 0, 1, 0);
	for(temp = c, temp.x++; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.x++)
		board_->highlight(temp.x, temp.y, 0, 1, 0);
	for(temp = c, temp.x--; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.x--)
		board_->highlight(temp.x, temp.y, 0, 1, 0);
	for(temp = c, temp.y++; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.y++)
		board_->highlight(temp.x, temp.y, 0, 1, 0);
	for(temp = c, temp.y--; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.y--)
		board_->highlight(temp.x, temp.y, 0, 1, 0);
}

bool tafl::game::is_possible_move(cell &from, cell &to)
{
	if(!from.is_valid() || !to.is_valid())
		return false;
	cell temp(from);
	if(from == to)
		return true;
	for(temp = from, temp.x++; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.x++)
		if(temp == to)
			return true;
	for(temp = from, temp.x--; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.x--)
		if(temp == to)
			return true;
	for(temp = from, temp.y++; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.y++)
		if(temp == to)
			return true;
	for(temp = from, temp.y--; temp.is_valid() && !board_->pawn_at(temp.x, temp.y); temp.y--)
		if(temp == to)
			return true;
	return false;
}

void tafl::game::input(void)
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			running_ = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_RIGHT)
				moving_camera_ = true;
			else
				on_mouse_down(e.button.button, e.button.x, e.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_RIGHT)
				moving_camera_ = false;
			break;
		case SDL_KEYUP:
			if(e.key.keysym.sym == SDLK_ESCAPE)
				running_ = false;
			else if(e.key.keysym.sym == SDLK_SPACE)
				camera_->home();
			break;
		case SDL_MOUSEMOTION:
			if(moving_camera_)
				dynamic_cast<centered_camera*>(camera_)->move(utils::deg_to_rad(e.motion.yrel)*0.5,
															  utils::deg_to_rad(e.motion.xrel)*0.5);
			else
				on_mouse_move(e.motion.x, e.motion.y);
			break;
		}
	}
}

void tafl::game::on_mouse_down(int button, int x, int y)
{
	if(button == SDL_BUTTON_LEFT)
	{
		cell c = board_->get_cell(x, y);
		if(c.is_valid())
		{
			if(selected_.is_valid() && is_possible_move(selected_, c))
			{
				board_->pawn_move(selected_.x, selected_.y, c.x, c.y);
				selected_.x = -1;
				selected_.y = -1;
			}
			else
				selected_ = c;
			board_->clear_highlight();
			if(board_->pawn_at(c.x, c.y))
				highlight_possible_moves(c);
			board_->highlight(c.x, c.y);
		}
	}
}
void tafl::game::on_mouse_move(int x, int y)
{

	cell c = board_->get_cell(x, y);
	board_->clear_highlight();
	if(selected_.is_valid() && board_->pawn_at(selected_.x, selected_.y))
	{
		highlight_possible_moves(selected_);
	}
	if(c.is_valid())
	{
		if(!selected_.is_valid() && board_->pawn_at(c.x, c.y))
		{
			highlight_possible_moves(c);
		}
		else
			if(selected_.is_valid()
			   && board_->pawn_at(selected_.x, selected_.y)
			   && !is_possible_move(selected_, c))
				board_->highlight(c.x, c.y, 1, 0, 0);
			else
				board_->highlight(c.x, c.y);
	}
}
