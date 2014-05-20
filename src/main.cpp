#include <exception>
#include <iostream>
#include <SDL.h>
#include <GL/gl.h>
#include "game.hpp"

int main(int argc, char** argv)
{
	try
	{
		/*
		 * Exception thrown in tafl level are fatal
		 * Other exceptions should be handled at lower level
		 */
		tafl::game tafl(argc, argv);
		return tafl();
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error occured", e.what(), NULL);
	}
	return -1;
}
