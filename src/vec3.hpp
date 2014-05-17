#ifndef VEC3_H_
#define VEC3_H_

namespace tafl
{
	struct vec3
	{
		float x;
		float y;
		float z;

		vec3(float x, float y, float z)
			:x(x) , y(y), z(z) {}
	};
}

#endif /* VEC3_H_ */
