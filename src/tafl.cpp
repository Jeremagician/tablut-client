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
{
	list<string> args;
	for(int i = 1; i < argc; i++)
		args.push_back(argv[i]);
}

tafl::~tafl()
{

}

int tafl::operator()()
{
	window window("Tablut", 800, 600);

	while(running_)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT)
				running_ = false;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		window.swap();
	}

	return EXIT_SUCCESS;
}
