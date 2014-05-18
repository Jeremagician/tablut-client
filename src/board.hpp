#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include "pawn_model.hpp"
#include "texture.hpp"

namespace tafl
{
	struct cell //external cell definition
	{
		int x;
		int y;

		cell(int x, int y, int w, int h) : x(x), y(y), w_(w), h_(h) {}
		cell(void) : x(0), y(0), w_(0), h_(0){}
		void set_bounds(int w, int h) { w_ = w; h_ = h;}
		bool is_valid(void) { return x >= 0 && y >= 0 && x < w_ && y < h_; }
	private:
		int w_, h_;
	};

	class board
	{
	public:
		board(int width, int height);
		virtual ~board(void);
		void render(void);

		/* Return cell pointed by (x,y)[screen coordinate]
		 * Invalid cell is inexistant */
		tafl::cell get_cell(int x, int y);
		void highlight(int x, int y, float r = 1.0, float g = 1.0, float b = 1.0);
		void clear_highlight(void);

	private:
		struct cell; // Internal cell definition
		struct pawn;

		const unsigned width_;
		const unsigned height_;
		void draw_highlight(int x, int y);
		void draw_board(void);

		cell **cells_;
		std::vector<pawn> pawns_;
		pawn *** grid_;

		/* init pawn rendering */
	    pawn_model muscovite{"data/pawns/muscovite.obj"};
	    pawn_model swedish{"data/pawns/swedish.obj"};
		pawn_model king{"data/pawns/king.obj"};
		texture board_tex{"data/board.jpg", texture::LINEAR, texture::REPEAT};
	};

	struct board::cell
	{
	    bool is_highlighted;
		float r,g,b; // highligh color
		cell(void) : is_highlighted(false) {}
	};

	struct board::pawn
	{
		int x, y;
		pawn_model &model;
		pawn(int x, int y, pawn_model &model) : x(x), y(y), model(model) {}
		void render(void);
	};
}

#endif /* BOARD_H_ */
