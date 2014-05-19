#ifndef FONT_H_
#define FONT_H_

#include <string>
#include <GL/gl.h>
#include <stb_truetype.h>

namespace tafl
{
	class font
	{
	public:
		font(std::string path, float size);
		virtual ~font();
		void render(float x, float y, std::string text);
	private:
		GLuint tex_id_;
		float size_;
		stbtt_bakedchar data_[96];
	};

}

#endif /* FONT_H_ */
