#pragma once

#include <vector>

#include "core/maths/maths.hpp"

namespace Core
{
	namespace Maths
	{
		enum class Collider
		{
			BOX,
			SPHERE,
		};

		class Box;
		class Sphere;

		class Primitives
		{
		protected:
			virtual void setAttribs() {};

		public:
			std::vector<vec3>			triangles;
			std::vector<unsigned int>	indices;
			Collider					collider;

			//a gameobject can only be a box or a sphere
			Box*		b = nullptr;
			Sphere*		sph = nullptr;

			void	setPrimPointersToNull();
		};
	}
}