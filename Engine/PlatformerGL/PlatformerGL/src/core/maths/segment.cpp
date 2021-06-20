#include "core/maths/segment.hpp"

Core::Maths::Segment::Segment(const vec3& s, const vec3& e)
{
	start = s;
	end = e;
	dir = pointVector(start, end);
}