#include "camera.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

tafl::camera::camera(
	vec3 pos,
	vec3 target,
	vec3 up,
	float aspect_ratio,
	float fov,
	float near,
	float far)
	: pos_(pos)
	, target_(target)
	, up_(up)
	, fov_(fov)
	, aspect_ratio_(aspect_ratio)
	, near_(near)
	, far_(far)
{

}

tafl::camera::~camera()
{

}

void tafl::camera::render(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov_, aspect_ratio_, near_, far_);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos_.x, pos_.y, pos_.z,
			  target_.x, target_.y, target_.z,
			  up_.x, up_.y, up_.z);
}
