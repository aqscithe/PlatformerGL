#include "core/maths/referential3.hpp"

Referential3::Referential3(vec3 origin)
	: origin(origin)
{
	i = { 1.f, 0.f, 0.f };
	j = { 0.f, 1.f, 0.f };
	k = { 0.f, 0.f, 1.f };
}

//void Referential3::rotate(Quaternion q)
//{
//	i.rlVec = Vector3RotateByQuaternion(i.rlVec, q);
//	j.rlVec = Vector3RotateByQuaternion(j.rlVec, q);
//	k.rlVec = Vector3RotateByQuaternion(k.rlVec, q);
//}