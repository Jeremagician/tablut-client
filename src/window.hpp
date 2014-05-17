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
};

#endif /* WINDOW_H_ */
