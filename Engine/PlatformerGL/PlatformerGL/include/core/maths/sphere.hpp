#pragma once

#include "core/maths/primitives.hpp"

namespace Core
{
	namespace Maths
	{
		class Sphere : public Primitives
		{
		private:
			void setAttribs() override;
			void setAttribs(const int lon, const int lat);

		public:
			vec3 omega;
			float radius;

			Sphere() = default;
			Sphere(const vec3& omega, const float& radius);
		};
	}
}