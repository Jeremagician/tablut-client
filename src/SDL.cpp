#include "SDL.hpp"
#include <SDL.h>

SDL::SDL(Uint32 flags) {

	if(rely_count == 0)
	{
		if(SDL_Init(0) != 0)
			throw InitError();
	}
	if(SDL_InitSubSystem(flags) != 0)
		throw InitError();
	rely_count++;
}

SDL::~SDL() {
	rely_count--;
	if(rely_count == 0)
		SDL_Quit();
}

SDL::InitError::InitError()
	: exception()
	, msg_(SDL_GetError())
{
}
SDL::InitError::~InitError()
{

}
const char* SDL::InitError::what()
{
	return msg_.c_str();
}

int SDL::rely_count = 0;
