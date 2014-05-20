#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/gl.h>
#include <string>

namespace tafl
{
	class texture
	{
	public:
		enum filtering
		{
			LINEAR = GL_LINEAR_MIPMAP_LINEAR,
			NEAREST = GL_NEAREST_MIPMAP_NEAREST
		};

		enum wrapping
		{
			REPEAT = GL_REPEAT,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT
		};

		texture(std::string filename, filtering filter = LINEAR, wrapping wrap = CLAMP_TO_BORDER);
	    ~texture();
		void bind(void);
	private:
		GLuint id_;
		std::string filename_;
		int width_;
		int height_;
	public:
#define getter(field)										\
		decltype(field##_) field(void) { return field##_; }
		getter(id);
		getter(filename);
		getter(width);
		getter(height);
#undef getter

	};
}


#endif /* TEXTURE_H_ */
