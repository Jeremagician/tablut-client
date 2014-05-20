#include "pawn_model.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

tafl::pawn_model::pawn_model(std::string path)
	: has_texture_(false)
{
	vector<tinyobj::shape_t> shapes;
	string err = tinyobj::LoadObj(shapes, path.c_str());
	if(!err.empty()) {
		throw runtime_error(err);
	}
	shape_ = shapes[0];

	if(!shape_.material.diffuse_texname.empty())
	{
		has_texture_ = true;
		texture_ = new texture(shape_.material.diffuse_texname, texture::LINEAR, texture::REPEAT);
	}

	create_display_list();
}

void tafl::pawn_model::create_display_list(void)
{
	display_list_id_ = glGenLists(1);
	glNewList(display_list_id_, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for(size_t f = 0; f < shape_.mesh.indices.size(); f++)
	{
		if(has_texture_)
			glTexCoord2f(shape_.mesh.texcoords[2*shape_.mesh.indices[f]+0],
						 shape_.mesh.texcoords[2*shape_.mesh.indices[f]+1]);

		glNormal3f(shape_.mesh.normals[3*shape_.mesh.indices[f]+0],
				   shape_.mesh.normals[3*shape_.mesh.indices[f]+1],
				   shape_.mesh.normals[3*shape_.mesh.indices[f]+2]);

		glVertex3f(shape_.mesh.positions[3*shape_.mesh.indices[f]+0],
				   shape_.mesh.positions[3*shape_.mesh.indices[f]+1],
				   shape_.mesh.positions[3*shape_.mesh.indices[f]+2]);
	}
	glEnd();
	glEndList();
}

tafl::pawn_model::~pawn_model()
{
	glDeleteLists(display_list_id_, 1);
}

void tafl::pawn_model::render(void)
{
	glPushMatrix();

	float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glMaterialf(GL_FRONT, GL_SHININESS,  128.0);

	if(has_texture_)
	{
		glEnable(GL_TEXTURE_2D);
		texture_->bind();
		glColor3f(1,1,1);
	}
	else
	{
		glColor3f(shape_.material.diffuse[0],
				  shape_.material.diffuse[1],
				  shape_.material.diffuse[2]);
	}


	glTranslatef(0.5,0.5,0);
	glScalef(0.04,0.04,0.04);
	glRotatef(90, 1, 0, 0);

	glCallList(display_list_id_);

	if(has_texture_)
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}
