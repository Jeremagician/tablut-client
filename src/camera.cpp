#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include "utils.hpp"
#include "camera.hpp"

tafl::camera::camera(
	vec3 pos,
	vec3 target,
	vec3 up,
	float fov,
	float aspect_ratio,
	float near,
	float far)
	: home_pos_(pos)
	, home_target_(target)
	, home_up_(up)
	, pos_(pos)
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

void tafl::camera::set_home(vec3 pos, vec3 target, vec3 up)
{
	home_pos_ = pos;
	home_target_ = target;
	home_up_ = up;
}

void tafl::camera::home(void)
{
	pos_ = home_pos_;
	target_ = home_target_;
	up_ = home_up_;
}

tafl::centered_camera::centered_camera(vec3 center, float radius, float vangle, float hangle,
									   vec3 up, float fov, float aspect, float near, float far)
	: camera(vec3(cos(vangle)*sin(hangle)*radius, cos(hangle)*cos(vangle)*radius, sin(vangle)*radius),
			 center, up, fov, aspect, near, far)
	, radius_(radius)
	, vertical_angle_(vangle)
	, horizontal_angle_(hangle)
	, home_vertical_angle_(vangle)
	, home_horizontal_angle_(hangle)
	, home_radius_(radius)
{

}

void tafl::centered_camera::home(void)
{
	camera::home();
	horizontal_angle_ = home_horizontal_angle_;
	vertical_angle_ = home_vertical_angle_;
	radius_ = home_radius_;
}

void tafl::centered_camera::move(float vertical_angle_diff, float horizontal_angle_diff)
{
	const float vertical_bounds = static_cast<float>(M_PI_2)-0.001;
	const float horizontal_bounds = static_cast<float>(M_PI_4)-0.001;

	vertical_angle_ += vertical_angle_diff;
	horizontal_angle_ += horizontal_angle_diff;

	vertical_angle_ = utils::clamp(vertical_angle_, 0.f, vertical_bounds);
	horizontal_angle_ = utils::clamp(horizontal_angle_, -horizontal_bounds, horizontal_bounds);

	pos_ = vec3(sin(horizontal_angle_)*cos(vertical_angle_)*radius_,
				cos(horizontal_angle_)*cos(vertical_angle_)*radius_, sin(vertical_angle_)*radius_);
}

tafl::centered_camera::~centered_camera(void)
{

}

void tafl::centered_camera::render(void)
{
	camera::render();
}
