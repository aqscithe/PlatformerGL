#include "core/maths/capsule.hpp"

Core::Maths::Capsule::Capsule(const vec3& center, const float& height, const float& radius, const Quaternion& q)
	:center(center), height(height), radius(radius), q(q)
{

}