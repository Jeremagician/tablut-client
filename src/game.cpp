#include <iostream>
#include <list>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <GL/gl.h>

#include "game.hpp"
#include "SDL.hpp"
#include "window.hpp"
#include "board.hpp"
#include "texture.hpp"

using namespace std;

tafl::game::game(int argc, char** argv)
	: speed_(0)
	, max_framerate_(80)
{
	list<string> args;
	int tw = 9, th = 9;
	for(int i = 1; i < argc; i++)
		args.push_back(argv[i]);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window_ = new window("Tablut", 1000, 800); // Can throw
	board_ = new tafl::board(tw,th); // Can trow aswell but safe
	camera_ = new tafl::camera(vec3(0,-tw/3.0, max(tw, th)),
							   vec3(0,0,0),
							   vec3(0,0,1),
							   static_cast<float>(window_->width())/window_->height(),
							   60);
	init_gl();
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

tafl::game::~game()
{
	delete camera_;
	delete board_;
	delete window_;
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
		if(SDL_GetTicks() - speed_time > 1000)
		{
			speed_time = SDL_GetTicks();
			speed_ = 1000.0/frames;
			frames = 0;
			cout << "FPS : " << 1000.0/speed_ << endl;
		}
	}

	return EXIT_SUCCESS;
}

void tafl::game::update(uint32_t diff)
{
	(void)diff;
}

void tafl::game::render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera_->render();
	board_->render();
	window_->swap();
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
			break;
		case SDL_KEYUP:
			if(e.key.keysym.sym == SDLK_ESCAPE)
				running_ = false;
			break;
		case SDL_MOUSEMOTION:
			cell c = board_->get_cell(e.motion.x, e.motion.y);
			board_->clear_highlight();
			if(c.is_valid())
				board_->highlight(c.x, c.y);
			break;
		}
	}
}
