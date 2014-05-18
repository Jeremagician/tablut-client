#ifndef TAFL_H_
#define TAFL_H_

#include "SDL.hpp"
#include "window.hpp"
#include "board.hpp"
#include "camera.hpp"
#include <cstdint>

namespace tafl
{
	class game
	{
	public:
		game(int argc, char** argv);
		virtual ~game(void);
		int operator()(void);
	private:
		void update(uint32_t diff);
		void render(void);
		void input(void);
		void init_gl(void);

		SDL sdl{SDL_INIT_EVERYTHING}; // Will init SDL automatically
		bool running_ = true;
		unsigned speed_;
		window *window_;
		board *board_;
		camera *camera_;
		unsigned max_framerate_;
	};
}

#endif /* TAFL_H_ */
