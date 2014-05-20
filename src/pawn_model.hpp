#ifndef PAWN_H_
#define PAWN_H_

#include <GL/gl.h>
#include <string>
#include "tiny_obj_loader.hpp"
#include "texture.hpp"

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
		texture *texture_;
		bool has_texture_;

		void create_display_list(void);

	public:
#define getter(field)										\
		decltype(field##_) field(void) { return field##_; }
		getter(has_texture);
#undef getter
	};

}

#endif /* PAWN_H_ */
