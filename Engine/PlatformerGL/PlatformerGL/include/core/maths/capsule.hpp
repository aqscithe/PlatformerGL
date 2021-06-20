#pragma once

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		class Capsule
		{
		public:
			vec3 center;
			float height;
			float radius;
			Quaternion q;

			Capsule() = default;
			Capsule(const vec3& center, const float& height, const float& radius, const Quaternion& q);
		};
	}
}