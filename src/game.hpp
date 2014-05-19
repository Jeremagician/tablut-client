#ifndef TAFL_H_
#define TAFL_H_

#include "SDL.hpp"
#include "window.hpp"
#include "board.hpp"
#include "camera.hpp"
#include "font.hpp"
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
		void highlight_possible_moves(cell &c);
		bool is_possible_move(cell &from, cell &to);

		void on_mouse_down(int x, int y);
		void on_mouse_move(int x, int y);

		SDL sdl{SDL_INIT_EVERYTHING}; // Will init SDL automatically
		tafl::font *font_;
		bool running_ = true;
		int speed_;
		window *window_;
		board *board_;
		camera *camera_;
		unsigned max_framerate_;
		cell selected_;
	};
}

#endif /* TAFL_H_ */
