#ifndef SDL_H_
#define SDL_H_

#include <SDL.h>
#include <exception>
#include <string>

class SDL final
{
public:
	SDL(Uint32 flags = 0);
	~SDL();

	class InitError : public std::exception {
	public:
		InitError();
		virtual ~InitError();
		const char* what();
	private:
		std::string msg_;
	};
private:
	static int rely_count; // How many people needs SDL ?
};

#endif /* SDL_H_ */
