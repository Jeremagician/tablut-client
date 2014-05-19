#include "font.hpp"
#include <fstream>
#include <memory>
#include <stdexcept>
#include <iostream>
using namespace std;

tafl::font::font(std::string path, float size)
	: size_(size)
{
	ifstream in;
	int length = 0;
	unique_ptr<char> buffer;
	unsigned char temp_bitmap[512*512]; //FIXME: Compute safe size
	(void)size;
	in.exceptions(ifstream::failbit | ifstream::badbit);
	in.open(path); // throw if can't
	in.seekg(0, in.end);
	length = in.tellg();
	in.seekg(0, in.beg);
	buffer = unique_ptr<char>(new char[length]);
	in.read(buffer.get(), length);

	stbtt_BakeFontBitmap(reinterpret_cast<unsigned char*>(buffer.get()), 0, size,
						 temp_bitmap, 512, 512,
						 32, 96, data_);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tex_id_);
	glBindTexture(GL_TEXTURE_2D, tex_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

tafl::font::~font()
{
	glDeleteTextures(1, &tex_id_);
}

void tafl::font::render(float x, float y, std::string text)
{
	glPushMatrix();
	y = y+size_;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id_);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	for(char c : text)
		if(c >= 32)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(data_, 512, 512, c-32, &x, &y, &q, 1);
			glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
			glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
			glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
			glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
		}
	glEnd();
	glPopMatrix();
}
