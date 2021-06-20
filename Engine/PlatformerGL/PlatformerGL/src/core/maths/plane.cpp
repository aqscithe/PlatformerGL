#include "core/maths/plane.hpp"

Core::Maths::Plane::Plane(const vec3& n, const float& d)
{
	normal = normalize(n);
	this->d = d;
}

Core::Maths::Plane::Plane(const vec3& n, const vec3& A)
{
	normal = normalize(n);
	d = dot(A, normal);
}

Core::Maths::Plane::Plane(const vec3& A, const vec3& B, const vec3& C)
{
	vec3 AB = pointVector(A, B);
	vec3 AC = pointVector(A, C);
	normal = normalize(vectProduct(AB, AC));
	d = dot(A, normal);
}