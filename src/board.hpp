#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <map>
#include <cstdint>
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
		bool operator==(cell &c) { return x==c.x && y==c.y; }
	private:
		int w_, h_;
	};

	class board
	{
	public:
		board(int width, int height);
		virtual ~board(void);
		void render(void);
		void update(uint32_t diff);

		/* Return cell pointed by (x,y)[screen coordinate]
		 * Invalid cell is inexistant */
		tafl::cell get_cell(int x, int y);
		void highlight(int x, int y, float r = 1.0, float g = 1.0, float b = 1.0);
		void unhighlight(int x, int y);
		void clear_highlight(void);
		/* temporary */
		enum pawn_type
		{
			MUSCOVITE,
			SWEDISH,
			KING
		};

		bool pawn_at(int x, int y);
		void pawn_add(int x, int y, pawn_type type);
		void pawn_remove(int x, int y);
		void pawn_move(int source_x, int source_y, int dest_x, int dest_y);

	private:
		struct cell; // Internal cell definition
		struct pawn;

		/* Matrix rendering time */
		GLdouble modelview_[16];
		GLdouble projection_[16];
		GLint viewport_[4];

		const unsigned width_;
		const unsigned height_;
		void draw_highlight(int x, int y);
		void draw_board(void);

		cell **cells_;
		std::vector<pawn> pawns_;

		std::map<pawn_type, pawn_model*> models_{
			{MUSCOVITE, new pawn_model("data/pawns/muscovite.obj")},
			{SWEDISH, new pawn_model("data/pawns/swedish.obj")},
			{KING, new pawn_model("data/pawns/king.obj")}
		};

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
		float real_x, real_y; // Used for animation
		float start_x, start_y; // Used for animation
		float percent;
		pawn_model *model;
		pawn(int x, int y, pawn_model *model);
		void move_to(int x, int y);
		void update(uint32_t diff);
		void render(void);
	};
}

#endif /* BOARD_H_ */
