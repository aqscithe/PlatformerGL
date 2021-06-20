#include "physics/transform.hpp"

using namespace Physics;
using namespace Core::Maths;

Transform::Transform(const vec3& position, const vec3& rotation, const vec3& scale): position(position), rotation(rotation), scale(scale)
{
	
}

Transform::Transform(const Transform& other)
{
	*this = other;
}

void Transform::operator=(const Transform& other)
{
	scale = other.scale;
	rotation = other.rotation;
	position = other.position;
}

mat4 Transform::getModelMatrix(float time)
{
	return translate(position) * rotateX(rotation.x)
		* rotateY(rotation.y) * rotateZ(rotation.z)
		* Core::Maths::scaleMatrix(scale);
}