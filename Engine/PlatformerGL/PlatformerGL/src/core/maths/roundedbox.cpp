#include "core/maths/roundedbox.hpp"

Core::Maths::RoundedBox::RoundedBox(const vec3& center, const vec4& extensions, const Quaternion& q)
	:center(center), extensions(extensions), q(q)
{
	float min = Maths::min(extensions.x, Maths::min(extensions.y, extensions.z));
	this->extensions.w = Maths::clamp(extensions.w, 0.f, min / 2);
}