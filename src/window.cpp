#include "window.hpp"
#include <SDL.h>

window::window(std::string title, int width, int height)
	: width_(width)
	, height_(height)
{
	int pos_x, pos_y;
	SDL_DisplayMode screen;

	SDL_GetCurrentDisplayMode(0, &screen);
	pos_x = screen.w/2.0 - width/2.0;
	pos_y = screen.h/2.0 - height/2.0;

	window_ = SDL_CreateWindow(
		title.c_str(), // Window title
		pos_x, // X position on the screen
		pos_y, // Y position on the screen
		width,
		height,
		SDL_WINDOW_OPENGL // Set window usable with OpenGL context
		);

	// Create a new OpenGL context
	context_ = SDL_GL_CreateContext(window_);
}

window::~window()
{
	SDL_GL_DeleteContext(context_);
	SDL_DestroyWindow(window_);
}

void window::swap(void)
{
	SDL_GL_SwapWindow(window_);
}
