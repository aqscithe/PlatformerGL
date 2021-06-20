#pragma once

#include "core/maths/maths.hpp"

namespace Physics
{
	using namespace Core::Maths;

	class Transform
	{
	public:
		Transform() = default;
		Transform(const vec3& position, const vec3& rotation, const vec3& scale);

		Transform(const Transform& other);

		void	operator=(const Transform& other);

		mat4	getModelMatrix(float time = 1.f);

		vec3	position{ 0.f, 0.f, 0.f };
		vec3	rotation{ 0.f, 0.f, 0.f };
		vec3	scale{ 1.f, 1.f, 1.f };
	};
}