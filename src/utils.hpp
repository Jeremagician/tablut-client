#ifndef UTILS_H_
#define UTILS_H_

namespace tafl
{
	namespace utils
	{
		template<typename T>
		constexpr T min(T A, T B)
		{
			return ((A < B) ? A : B);
		}

		template<typename T>
		constexpr T max(T A, T B)
		{
			return ((A > B) ? A : B);
		}

		template<typename T>
		constexpr T clamp(T value, T min, T max)
		{
			return utils::max(min, utils::min(value, max));
		}

		/* Linear interpolation */
		template<typename T>
		constexpr T lerp(T start, T end, T alpha)
		{
			return (start + alpha*(end-start));
		}

		/* Smooth interpolation */
		template<typename T>
		T serp(T start, T end, T alpha)
		{
			alpha = clamp(alpha, 0.0f, 1.0f);
			alpha = alpha*alpha*alpha*(alpha*(alpha*6 - 15) + 10);
			return lerp(start, end, alpha);
		}

		constexpr float deg_to_rad(float deg)
		{
			return deg * 0.0174532925;
		}
	}
}


#endif /* UTILS_H_ */
