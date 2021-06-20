#pragma once

#include "core/maths/maths.hpp"
#include "core/core.hpp"

#include <vector>

#include "core/maths/primitives.hpp"

namespace Core
{
	namespace Maths
	{
		class Box : public Primitives
		{
		private:
			void setAttribs() override;

		public:
			vec3 center;
			vec3 extensions; //width, height, depth
			Quaternion q;

			Box() = default;
			Box(const vec3& center, const vec3& extensions, const Quaternion& q);
		};
	}
}