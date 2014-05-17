#include <iostream>
#include <list>
#include <exception>
#include <stdexcept>
#include <GL/gl.h>

#include "tafl.hpp"
#include "SDL.hpp"
#include "window.hpp"

using namespace std;

tafl::tafl(int argc, char** argv)
	: max_framerate_(80)
{
	list<string> args;
	for(int i = 1; i < argc; i++)
		args.push_back(argv[i]);

	window_ = new window("Tablut", 800, 600); // Can throw
}

tafl::~tafl()
{
	delete window_;
}

int tafl::operator()()
{
	unsigned last_time, current_time, diff;
	unsigned ms_per_frame = 1000.0/max_framerate_;

	last_time = SDL_GetTicks();
	while(running_)
	{
		current_time = SDL_GetTicks();
		diff = current_time - last_time;

		input();
		update(diff);
		render();

		last_time = current_time;
		/* Sleep if we are rendering too much */
		diff = SDL_GetTicks() - last_time;
		if(diff < ms_per_frame)
			SDL_Delay(ms_per_frame-diff);
	}

	return EXIT_SUCCESS;
}

void tafl::update(uint32_t diff)
{
	(void)diff;
}

void tafl::render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window_->swap();
}

void tafl::input(void)
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
