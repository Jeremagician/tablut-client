#ifndef BOARD_H_
#define BOARD_H_

namespace tafl
{
	class board
	{
	public:
		board(int width, int height);
		virtual ~board(void);
		void render(void);

	private:
		const unsigned width_;
		const unsigned height_;
	};
}

#endif /* BOARD_H_ */
