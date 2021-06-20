#pragma once

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		class Quad
		{
		public:
			vec3 center;
			vec2 extensions; //width, height
			Quaternion q;
			bool reverse = false;

			Quad() = default;
			Quad(const vec3& center, const vec2& extensions, const Quaternion& q, const bool reverse);

			vec3 getNormal() const;
		};
	}
}