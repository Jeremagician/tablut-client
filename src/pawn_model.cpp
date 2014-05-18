#include "pawn_model.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

tafl::pawn_model::pawn_model(std::string path)
{
	vector<tinyobj::shape_t> shapes;
	string err = tinyobj::LoadObj(shapes, path.c_str());
	if(!err.empty()) {
		throw runtime_error(err);
	}
	shape_ = shapes[0];

	display_list_id_ = glGenLists(1);
	glNewList(display_list_id_, GL_COMPILE);

	glBegin(GL_TRIANGLES);
	for(size_t f = 0; f < shape_.mesh.indices.size(); f++)
	{
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
	glTranslatef(0.5,0.5,0);
	glScalef(0.04,0.04,0.04);
	glRotatef(90, 1, 0, 0);

	glColor3f(shape_.material.diffuse[0],
			  shape_.material.diffuse[1],
			  shape_.material.diffuse[2]);

	glCallList(display_list_id_);

	glPopMatrix();

}
