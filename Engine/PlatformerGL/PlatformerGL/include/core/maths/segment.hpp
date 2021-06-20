#pragma once

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		class Segment
		{
		public:
			vec3 start, end;
			vec3 dir;

			Segment() = default;
			Segment(const vec3& s, const vec3& e);
		};
	}
}