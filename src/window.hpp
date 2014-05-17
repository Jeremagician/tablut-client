#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <SDL.h>

class window
{
public:
	window(std::string title, int width, int height);
	virtual ~window(void);
	void swap(void);
private:
	SDL_Window *window_;
	SDL_GLContext context_;
	int width_, height_;

public:
#define getter(field)									\
	decltype(field##_) field(void) { return field##_; }

	getter(width);
	getter(height);
#undef getter
};

#endif /* WINDOW_H_ */
