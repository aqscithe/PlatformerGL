#include "core/maths/quad.hpp"

using namespace Core::Maths;

Core::Maths::Quad::Quad(const vec3& center, const vec2& extensions, const Quaternion& q, const bool reverse)
	:center(center), extensions(extensions), q(q), reverse(reverse)
{

}

vec3 Core::Maths::Quad::getNormal() const
{
    //get quad's normal through its quaternion
    vec3 normal = { 0.f, 1.f, 0.f };
    normal = Vector3RotateByQuaternion(normal, q);

    if (reverse)
        normal = -normal;

    return normal;
}