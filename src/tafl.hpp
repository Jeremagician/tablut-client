#ifndef TAFL_H_
#define TAFL_H_

#include "SDL.hpp"
#include "window.hpp"
#include <cstdint>

class tafl
{
public:
	tafl(int argc, char** argv);
	virtual ~tafl(void);
	int operator()(void);
private:
	void update(uint32_t diff);
	void render(void);
	void input(void);

	SDL sdl{SDL_INIT_EVERYTHING}; // Will init SDL automatically
	bool running_ = true;
	window *window_;
	unsigned max_framerate_;
};



#endif /* TAFL_H_ */
