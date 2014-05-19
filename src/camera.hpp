#ifndef CAMERA_H_
#define CAMERA_H_

#include "vec3.hpp"

namespace tafl
{
	class camera
	{
	public:
		camera(vec3 pos = vec3(0,0,0),
			   vec3 target = vec3(0,0,0),
			   vec3 up = vec3(0,0,1),
			   float fov = 60,
			   float aspect_ratio_ = 1.0,
			   float near = 0.001,
			   float far = 1000.0
			);

		virtual ~camera();
		virtual void render(void);
		virtual void set_home(vec3 pos, vec3 target, vec3 up);
		virtual void home(void);
	protected:
		vec3 home_pos_;
		vec3 home_target_;
		vec3 home_up_;

		vec3 pos_;
		vec3 target_;
		vec3 up_;
		float fov_;
		float aspect_ratio_;
		float near_;
		float far_;
	public:
#define getter(field) \
		virtual decltype(field##_) field(void) { return field##_; }

		getter(pos);
		getter(target);
		getter(up);
		getter(fov);
		getter(aspect_ratio);
		getter(near);
		getter(far);
#undef getter

#define setter(field) \
		virtual void set_##field(decltype(field##_) &field) { field##_ = field; }

		setter(pos);
		setter(target);
		setter(up);
		setter(fov);
		setter(aspect_ratio);
		setter(near);
		setter(far);
#undef setter
	};

	class centered_camera : public camera
	{
	public:
		centered_camera(vec3 center, float radius, float vangle = 0, float hangle = 0,
						vec3 up = vec3(0,0,1), float fov = 60, float aspect = 1.0, float near = 0.001, float far = 1000.0 );
		virtual ~centered_camera(void);
		virtual void move(float vertical_angle_diff, float horizontal_angle_diff);
		virtual void render(void);
		virtual void home(void);
	private:
		float radius_;
		float vertical_angle_;
		float horizontal_angle_;

		float home_vertical_angle_;
		float home_horizontal_angle_;
		float home_radius_;
	};
}

#endif /* CAMERA_H_ */
