#include <iostream>
#include <list>
#include <exception>
#include <stdexcept>
#include <GL/gl.h>

#include "game.hpp"
#include "SDL.hpp"
#include "window.hpp"
#include "board.hpp"

using namespace std;

tafl::game::game(int argc, char** argv)
	: speed_(0)
	, max_framerate_(80)
{
	list<string> args;
	for(int i = 1; i < argc; i++)
		args.push_back(argv[i]);

	window_ = new window("Tablut", 800, 600); // Can throw
	board_ = new tafl::board(9,9); // Can trow aswell but safe
	camera_ = new tafl::camera(vec3(0,-2,1),
							   vec3(0,0,0),
							   vec3(0,0,1),
							   static_cast<float>(window_->width())/window_->height(),
							   60);
}

tafl::game::~game()
{
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
			cout << "MPF : " << speed_ << endl;
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
			cout << "Click at (" << e.button.x << ", " << e.button.y << ")" << endl;
			break;
		case SDL_KEYUP:
			if(e.key.keysym.sym == SDLK_ESCAPE)
				running_ = false;
			break;
		}
	}
}
