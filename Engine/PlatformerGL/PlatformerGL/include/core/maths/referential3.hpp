#pragma once

#include "maths.hpp"

using namespace Core::Maths;

class Referential3
{
public:
	vec3 origin;
	vec3 i, j, k;

	Referential3() = default;
	Referential3(vec3 origin);

	//void rotate(Quaternion q);
};