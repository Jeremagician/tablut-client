#ifndef PAWN_H_
#define PAWN_H_

#include <GL/gl.h>
#include <string>
#include "tiny_obj_loader.hpp"

namespace tafl
{
	class pawn_model
	{
	public:
		pawn_model(std::string path);
		virtual ~pawn_model();
		void render(void);
	private:
		tinyobj::shape_t shape_;
		GLuint display_list_id_;
	};

}

#endif /* PAWN_H_ */
