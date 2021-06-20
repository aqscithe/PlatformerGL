#pragma once

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		class RoundedBox
		{
		public:
			vec3 center;
			vec4 extensions; //width, heigth, length, radius
			Quaternion q;

			RoundedBox() = default;
			RoundedBox(const vec3& center, const vec4& extensions, const Quaternion& q);
		};
	}
}