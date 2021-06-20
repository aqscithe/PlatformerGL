#pragma once

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		class Plane
		{
		public:
			vec3	normal;
			float	d;

			Plane() = default;
			Plane(const vec3& n, const float& d);
			Plane(const vec3& n, const vec3& A);
			Plane(const vec3& A, const vec3& B, const vec3& C);
		};
	}
}