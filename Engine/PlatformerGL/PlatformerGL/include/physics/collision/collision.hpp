#pragma once

#include "core/maths/maths.hpp"
#include "core/maths/segment.hpp"
#include "core/maths/plane.hpp"
#include "core/maths/sphere.hpp"
#include "core/maths/quad.hpp"
#include "core/maths/box.hpp"
#include "core/maths/cylinder.hpp"
#include "core/maths/capsule.hpp"
#include "core/maths/roundedbox.hpp"

namespace Physics
{
	using namespace Core::Maths;

	namespace Collision
	{
		bool cSegmentPlane(const Segment& s, const Plane& p, vec3& intersection, vec3& normal);

		bool cSegmentSphere(const Segment& seg, const Sphere& sph, vec3& intersection, vec3& normal);

		bool cSegmentCylinderInf(const Segment& s, const Cylinder& cy, vec3& intersection, vec3& normal);
		bool cSegmentCylinder(const Segment& s, const Cylinder& c, vec3& intersection, vec3& normal);

		bool cSegmentQuad(const Segment& s, const Quad& q, vec3& intersection, vec3& normal);

		bool cSegmentBox(const Segment& s, const Box& b, vec3& intersection, vec3& normal);
		bool cSegmentBox(const Segment& s, const Box& b, float radius, vec3& intersection, vec3& normal);

		bool cSegmentCapsule(const Segment& s, const Capsule& c, vec3& intersection, vec3& normal);

		bool cSegmentRoundedBox(const Segment& s, const RoundedBox& rb, vec3& intersection, vec3& normal);

		bool cBoxSphere(const Box& b, const Sphere& s, vec3& intersection);
		bool cSphereSphere(const Sphere& s1, const Sphere& s2);
	}
}