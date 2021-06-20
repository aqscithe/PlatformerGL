#include "physics/collision/collision.hpp"

#include "core/maths/referential3.hpp"

#include <iostream>

bool Physics::Collision::cSegmentPlane(const Segment& s, const Plane& p, vec3& intersection, vec3& normal)
{
	float segPlan = dot(s.dir, p.normal);
	if (segPlan == 0)
		return false;

	vec3 P = p.normal * p.d;
	float t = (dot(pointVector(s.start, P), p.normal)) / segPlan;

	if (t < 0 || t > 1)
		return false;

	//drawing intersection point
	intersection = (s.start + s.dir * t);
	//DrawSphere(intersection.rlVec, 0.1f, RED);
	//DrawLine3D(intersection.rlVec, (intersection + p.normal).rlVec, BLUE);

	normal = p.normal;
	return true;
}

bool Physics::Collision::cSegmentSphere(const Segment& seg, const Sphere& sph, vec3& intersection, vec3& normal)
{
	float segMag = mag(seg.dir);
	vec3 segSph = pointVector(sph.omega, seg.start);
	float segSphMag = mag(segSph);

	float a = segMag * segMag;
	float b = 2 * dot(segSph, seg.dir);
	float c = segSphMag * segSphMag - sph.radius * sph.radius;

	float delta = b * b - 4 * a * c;

	if (delta < 0)
		return false;

	float t = (-b - sqrtf(delta)) / (2 * a);

	if (t < 0 || t > 1)
		return false;

	//drawing intersection point
	intersection = (segSph + seg.dir * t) + sph.omega;
	//DrawSphere(intersection.rlVec, 0.1f, RED);
	normal = normalize(pointVector(sph.omega, intersection));
	//DrawLine3D(intersection.rlVec, (intersection + normal).rlVec, BLUE);
	return true;
}

bool Physics::Collision::cSegmentCylinderInf(const Segment& s, const Cylinder& cy, vec3& intersection, vec3& normal)
{
	vec3 A = { 0.f + cy.height / 2, 0.f, 0.f };
	vec3 B = { 0.f - cy.height / 2, 0.f, 0.f };

	A = Vector3RotateByQuaternion(A, cy.q);
	B = Vector3RotateByQuaternion(B, cy.q);

	translate(A, cy.center);
	translate(B, cy.center);

	vec3 PQ = pointVector(A, B);
	vec3 AB = s.dir;
	vec3 PA = pointVector(A, s.start);

	float a = dot(PQ, PQ) * dot(AB, AB) - dot(PQ, AB) * dot(PQ, AB);
	float b = dot(PQ, PQ) * dot(PA, AB) - dot(PQ, AB) * dot(PA, PQ);
	float c = dot(PQ, PQ) * (dot(PA, PA) - cy.radius * cy.radius) - dot(PA, PQ) * dot(PA, PQ);

	float delta = b * b - a * c;

	if (delta < 0)
		return false;

	float t = (-b - sqrtf(delta)) / a;

	if (t < 0 || t > 1)
		return false;

	//drawing intersection point
	intersection = s.start + s.dir * t;
	//DrawSphere(intersection.rlVec, 0.1f, RED);
	vec3 PM = pointVector(A, intersection);
	vec3 HM = (PQ * (dot(PM, PQ))) / dot(PQ, PQ) - PM;
	HM = normalize(HM);
	normal = HM;
	//DrawLine3D(intersection.rlVec, (intersection - HM).rlVec, BLUE);
	return true;
}

bool Physics::Collision::cSegmentCylinder(const Segment& s, const Cylinder& c, vec3& intersection, vec3& normal)
{
	if (!cSegmentCylinderInf(s, c, intersection, normal))
		return false;

	vec3 A = { 0.f + c.height / 2, 0.f, 0.f };
	vec3 B = { 0.f - c.height / 2, 0.f, 0.f };

	A = Vector3RotateByQuaternion(A, c.q);
	B = Vector3RotateByQuaternion(B, c.q);

	translate(A, c.center);
	translate(B, c.center);

	vec3 PM = pointVector(A, intersection);
	vec3 PQ = pointVector(A, B);

	if (!(dot(PM, PQ) < 0 || dot(PM, PQ) > dot(PQ, PQ)))
	{
		vec3 HM = PM - (PQ * (dot(PM, PQ))) / dot(PQ, PQ);
		HM = normalize(HM);
		normal = HM;

		return true;
	}

	Plane discA(-PQ, A);
	Plane discB(PQ, B);

	if (cSegmentPlane(s, discA, intersection, normal))
	{
		PM = pointVector(A, intersection);
		if (dot(PM, PM) < c.radius * c.radius)
			return true;
	}
	else if (cSegmentPlane(s, discB, intersection, normal))
	{
		PM = pointVector(B, intersection);
		if (dot(PM, PM) < c.radius * c.radius)
			return true;
	}

	return false;
}

bool Physics::Collision::cSegmentQuad(const Segment& s, const Quad& q, vec3& intersection, vec3& normal)
{
	Plane p(q.getNormal(), q.center);
	if (!cSegmentPlane(s, p, intersection, normal))
		return false;

	if (intersection.x > q.center.x + q.extensions.x / 2.f || intersection.x < q.center.x - q.extensions.x / 2.f)
		return false;
	if (intersection.y > q.center.y + q.extensions.y / 2.f || intersection.y < q.center.y - q.extensions.y / 2.f)
		return false;
	if (intersection.z > q.center.z + q.extensions.y / 2.f || intersection.z < q.center.z - q.extensions.y / 2.f)
		return false;

	return true;
}

bool Physics::Collision::cSegmentBox(const Segment& s, const Box& b, vec3& intersection, vec3& normal)
{
	vec3 A = { 0.f + b.extensions.x / 2, 0.f, 0.f };
	vec3 B = { 0.f - b.extensions.x / 2, 0.f, 0.f };
	vec3 C = { 0.f, 0.f + b.extensions.y / 2, 0.f };
	vec3 D = { 0.f, 0.f - b.extensions.y / 2, 0.f };
	vec3 E = { 0.f, 0.f, 0.f + b.extensions.z / 2 };
	vec3 F = { 0.f, 0.f, 0.f - b.extensions.z / 2 };

	A = Vector3RotateByQuaternion(A, b.q);
	B = Vector3RotateByQuaternion(B, b.q);
	C = Vector3RotateByQuaternion(C, b.q);
	D = Vector3RotateByQuaternion(D, b.q);
	E = Vector3RotateByQuaternion(E, b.q);
	F = Vector3RotateByQuaternion(F, b.q);

	translate(A, b.center);
	translate(B, b.center);
	translate(C, b.center);
	translate(D, b.center);
	translate(E, b.center);
	translate(F, b.center);

	Quad quads[6];
	quads[0] = { A,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90 * TORAD)), true };

	quads[1] = { B,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90 * TORAD)), false };

	quads[2] = { C,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0 * TORAD)), false };

	quads[3] = { D,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0 * TORAD)), true };

	quads[4] = { E,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90 * TORAD)), true };

	quads[5] = { F,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90 * TORAD)), false };

	for (int i = 0; i < 6; ++i)
	{
		if (cSegmentQuad(s, quads[i], intersection, normal))
			return true;
	}

	return false;
}

bool Physics::Collision::cSegmentBox(const Segment& s, const Box& b, float radius, vec3& intersection, vec3& normal)
{
	vec3 A = { 0.f + b.extensions.x / 2.f - radius, 0.f, 0.f };
	vec3 B = { 0.f - b.extensions.x / 2.f + radius, 0.f, 0.f };
	vec3 C = { 0.f, 0.f + b.extensions.y / 2.f - radius, 0.f };
	vec3 D = { 0.f, 0.f - b.extensions.y / 2.f + radius, 0.f };
	vec3 E = { 0.f, 0.f, 0.f + b.extensions.z / 2.f - radius };
	vec3 F = { 0.f, 0.f, 0.f - b.extensions.z / 2.f + radius };

	A = Vector3RotateByQuaternion(A, b.q);
	B = Vector3RotateByQuaternion(B, b.q);
	C = Vector3RotateByQuaternion(C, b.q);
	D = Vector3RotateByQuaternion(D, b.q);
	E = Vector3RotateByQuaternion(E, b.q);
	F = Vector3RotateByQuaternion(F, b.q);

	translate(A, b.center);
	translate(B, b.center);
	translate(C, b.center);
	translate(D, b.center);
	translate(E, b.center);
	translate(F, b.center);

	Quad quads[6];
	quads[0] = { A,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90.f * TORAD)), true };

	quads[1] = { B,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90.f * TORAD)), false };

	quads[2] = { C,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0.f * TORAD)), false };

	quads[3] = { D,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0.f * TORAD)), true };

	quads[4] = { E,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90.f * TORAD)), true };

	quads[5] = { F,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90.f * TORAD)), false };

	for (int i = 0; i < 6; ++i)
	{
		if (cSegmentQuad(s, quads[i], intersection, normal))
			return true;
	}

	return false;
}

bool Physics::Collision::cSegmentCapsule(const Segment& s, const Capsule& c, vec3& intersection, vec3& normal)
{
	vec3 A = { 0.f + c.height / 2, 0.f, 0.f };
	vec3 B = { 0.f - c.height / 2, 0.f, 0.f };

	A = Vector3RotateByQuaternion(A, c.q);
	B = Vector3RotateByQuaternion(B, c.q);

	translate(A, c.center);
	translate(B, c.center);

	Cylinder cy(c.center, c.height, c.radius, c.q);

	if (cSegmentCylinder(s, cy, intersection, normal))
		return true;
	else
	{
		Sphere sph1 = { A, c.radius };
		Sphere sph2 = { B, c.radius };

		if (cSegmentSphere(s, sph1, intersection, normal))
			return true;
		if (cSegmentSphere(s, sph2, intersection, normal))
			return true;
	}
	normal = { 0,0,0 };

	return false;
}

bool Physics::Collision::cSegmentRoundedBox(const Segment& s, const RoundedBox& rb, vec3& intersection, vec3& normal)
{
	if (dot(s.dir, pointVector(s.start, rb.center)) <= 0)
		return false;

	Box b = { rb.center, rb.extensions.xyz, rb.q };

	if (cSegmentBox(s, b, rb.extensions.w, intersection, normal))
		return true;

	return false;

	float radius = rb.extensions.w;

	Quaternion k = QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 90.f * TORAD);
	Quaternion l = QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90.f * TORAD);
	Quaternion q1 = QuaternionMultiply(rb.q, k);
	Quaternion q2 = QuaternionMultiply(rb.q, l);

	vec3 A = { 0.f, 0.f + rb.extensions.y / 2.f - radius, 0.f + rb.extensions.z / 2.f - radius };
	vec3 B = { 0.f, 0.f - rb.extensions.y / 2.f + radius, 0.f - rb.extensions.z / 2.f + radius };
	vec3 C = { 0.f, 0.f - rb.extensions.y / 2.f + radius, 0.f + rb.extensions.z / 2.f - radius };
	vec3 D = { 0.f, 0.f - rb.extensions.y / 2.f + radius, 0.f + rb.extensions.z / 2.f - radius };
	vec3 E = { 0.f, 0.f + rb.extensions.y / 2.f - radius, 0.f - rb.extensions.z / 2.f + radius };
	vec3 F = { 0.f, 0.f + rb.extensions.y / 2.f - radius, 0.f + rb.extensions.z / 2.f - radius };
	vec3 G = { 0.f, 0.f - rb.extensions.y / 2.f + radius, 0.f + rb.extensions.z / 2.f - radius };
	vec3 H = { 0.f, 0.f - rb.extensions.y / 2.f + radius, 0.f - rb.extensions.z / 2.f + radius };
	vec3 I = { 0.f, 0.f - rb.extensions.y / 2.f + radius, 0.f - rb.extensions.z / 2.f + radius };
	vec3 J = { 0.f, 0.f + rb.extensions.y / 2.f - radius, 0.f + rb.extensions.z / 2.f - radius };
	vec3 K = { 0.f, 0.f + rb.extensions.y / 2.f - radius, 0.f - rb.extensions.z / 2.f + radius };
	vec3 L = { 0.f, 0.f + rb.extensions.y / 2.f - radius, 0.f - rb.extensions.z / 2.f + radius };

	A = Vector3RotateByQuaternion(A, q1);
	B = Vector3RotateByQuaternion(B, q2);
	C = Vector3RotateByQuaternion(C, q1);
	D = Vector3RotateByQuaternion(D, q2);
	E = Vector3RotateByQuaternion(E, rb.q);
	F = Vector3RotateByQuaternion(F, rb.q);
	G = Vector3RotateByQuaternion(G, rb.q);
	H = Vector3RotateByQuaternion(H, rb.q);
	I = Vector3RotateByQuaternion(I, q1);
	J = Vector3RotateByQuaternion(J, q2);
	K = Vector3RotateByQuaternion(K, q1);
	L = Vector3RotateByQuaternion(L, q2);

	translate(A, rb.center);
	translate(B, rb.center);
	translate(C, rb.center);
	translate(D, rb.center);
	translate(E, rb.center);
	translate(F, rb.center);
	translate(G, rb.center);
	translate(H, rb.center);
	translate(I, rb.center);
	translate(J, rb.center);
	translate(K, rb.center);
	translate(L, rb.center);

	Capsule c[12];
	c[0] = { A, rb.extensions.z - radius * 2.f, radius, q1 };
	c[1] = { B, rb.extensions.y - radius * 2.f, radius, q2 };
	c[2] = { C, rb.extensions.z - radius * 2.f, radius, q1 };
	c[3] = { D, rb.extensions.y - radius * 2.f, radius, q2 };
	c[4] = { E, rb.extensions.x - radius * 2.f, radius, rb.q };
	c[5] = { F, rb.extensions.x - radius * 2.f, radius, rb.q };
	c[6] = { G, rb.extensions.x - radius * 2.f, radius, rb.q };
	c[7] = { H, rb.extensions.x - radius * 2.f, radius, rb.q };
	c[8] = { I, rb.extensions.z - radius * 2.f, radius, q1 };
	c[9] = { J, rb.extensions.y - radius * 2.f, radius, q2 };
	c[10] = { K, rb.extensions.z - radius * 2.f, radius, q1 };
	c[11] = { L, rb.extensions.y - radius * 2.f, radius, q2 };

	for (int i = 0; i < 12; ++i)
	{
		if (cSegmentCapsule(s, c[i], intersection, normal))
			return true;
	}

	return false;
}

bool Physics::Collision::cBoxSphere(const Box& b, const Sphere& s, vec3& intersection)
{
	//local referencial of the box
	Referential3 local(b.center);
	vec3 OP = pointVector(local.origin, s.omega);
	//local.rotate(b.q);

	vec3 localOmega = { dot(OP, local.i), dot(OP, local.j), dot(OP, local.k) };

	if (fabsf(localOmega.x) > b.extensions.x / 2 + s.radius ||
		fabsf(localOmega.y) > b.extensions.y / 2 + s.radius ||
		fabsf(localOmega.z) > b.extensions.z / 2 + s.radius)
		return false;

	vec3 closest = { clamp(localOmega.x, -b.extensions.x / 2, b.extensions.x / 2),
					 clamp(localOmega.y, -b.extensions.y / 2, b.extensions.y / 2),
					 clamp(localOmega.z, -b.extensions.z / 2, b.extensions.z / 2) };
	float magOP = sqrMag(pointVector(localOmega, closest));

	if (magOP > s.radius * s.radius)
		return false;

	Referential3 global({ 0.f, 0.f, 0.f });
	intersection = b.center + closest;

	return true;
}

bool Physics::Collision::cSphereSphere(const Sphere& s1, const Sphere& s2)
{
	//vector between each center
	vec3 OP = pointVector(s1.omega, s2.omega);

	float sqrRadiuses = (s1.radius + s2.radius) * (s1.radius + s2.radius);
	float magOP = sqrMag(OP);

	if (magOP > sqrRadiuses)
		return false;

	return true;
}