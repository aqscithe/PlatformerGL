#pragma once

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		class Cylinder
		{
		public:
			vec3 center;
			float height;
			float radius;
			Quaternion q;

			Cylinder() = default;
			Cylinder(const vec3& center, const float& height, const float& radius, const Quaternion& q);
		};
	}
}