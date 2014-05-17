#ifndef TAFL_H_
#define TAFL_H_

#include "SDL.hpp"

class tafl
{
public:
	tafl(int argc, char** argv);
	virtual ~tafl();
	int operator()();
private:
	SDL sdl{SDL_INIT_EVERYTHING}; // Will init SDL automatically
	bool running_ = true;
};



#endif /* TAFL_H_ */
