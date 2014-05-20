#include <GL/gl.h>
#include <GL/glu.h>
#include "texture.hpp"
#include <stb_image.h>
#include <stdexcept>
#include <iostream>
using namespace std;

tafl::texture::texture(std::string filename, filtering filter, wrapping wrap)
	: filename_(filename)
{
	unsigned char *data;

	data = stbi_load(filename.c_str(), &width_, &height_, NULL, 4);
	if(data == NULL)
		throw runtime_error("Can't load texture : " + filename);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void tafl::texture::bind(void)
{
	glBindTexture(GL_TEXTURE_2D, id_);
}

tafl::texture::~texture()
{
	glDeleteTextures(1, &id_);
}
