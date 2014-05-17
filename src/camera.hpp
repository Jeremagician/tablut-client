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
		void render(void);

	private:
		vec3 pos_;
		vec3 target_;
		vec3 up_;
		float fov_;
		float aspect_ratio_;
		float near_;
		float far_;

	public:
#define getter(field) \
		decltype(field##_) field(void) { return field##_; }

		getter(pos);
		getter(target);
		getter(up);
		getter(fov);
		getter(aspect_ratio);
		getter(near);
		getter(far);
#undef getter

#define setter(field) \
		void set_##field(decltype(field##_) &field) { field##_ = field; }

		setter(pos);
		setter(target);
		setter(up);
		setter(fov);
		setter(aspect_ratio);
		setter(near);
		setter(far);
#undef setter
	};
}

#endif /* CAMERA_H_ */
