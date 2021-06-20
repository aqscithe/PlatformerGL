#include "core/maths/maths.hpp"

#include <cmath>

using namespace Core::Maths;

mat4 Core::Maths::translateMatrix(const vec3& pos)
{
	return mat4{
		1.f, 0.f, 0.f, pos.x,
		0.f, 1.f, 0.f, pos.y,
		0.f, 0.f, 1.f, pos.z,
		0.f, 0.f, 0.f, 1.f,
	};
}

mat4 Core::Maths::rotateXMatrix(const float& rad)
{
	return mat4{
		1.f, 0.f, 0.f, 0.f,
		0.f, cos(rad), -sin(rad), 0.f,
		0.f, sin(rad), cos(rad), 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
}

mat4 Core::Maths::rotateYMatrix(const float& rad)
{
	return mat4{
		cos(rad), 0.f, sin(rad), 0.f,
		0.f, 1.f, 0.f, 0.f,
		-sin(rad), 0.f, cos(rad), 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
}

mat4 Core::Maths::rotateZMatrix(const float& rad)
{
	return mat4{
		cos(rad), -sin(rad), 0.f, 0.f,
		sin(rad), cos(rad), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
}

mat4 Core::Maths::scaleMatrix(const vec3& scale)
{
	return mat4{
		scale.x, 0.f, 0.f, 0.f,
		0.f, scale.y, 0.f, 0.f,
		0.f, 0.f, scale.z, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
}

void Core::Maths::translate(vec3& dest, const vec3& t)
{
	dest.x += t.x;
	dest.y += t.y;
	dest.z += t.z;
}

vec3 Core::Maths::pointVector(const vec3& a, const vec3& b)
{
	return { b.x - a.x, b.y - a.y, b.z - a.z };
}

bool Core::Maths::operator!=(const vec3& a, const vec3& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

float Core::Maths::dot(const vec3& a, const vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 Core::Maths::vectProduct(const vec3& a, const vec3& b)
{
	vec3 temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.x * b.z - a.z * b.x;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

float Core::Maths::mag(const vec3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Core::Maths::sqrMag(const vec3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

vec3 Core::Maths::normalize(const vec3& v)
{
    float mag = Maths::mag(v);
	return v / (mag == 0.f ? 1.f : mag);
}

void Core::Maths::sortAscendingFloat(float* v, const int num, int* order)
{
    for (int i = 0; i < num; ++i)
    {
        for (int j = i; j < num; ++j)
        {
            if (v[j] < v[i])
            {
                //floats
                float temp = v[i];

                v[i] = v[j];
                v[j] = temp;

                //ints
                int t = order[i];

                order[i] = order[j];
                order[j] = t;
            }
        }
    }
}

Quaternion Core::Maths::QuaternionIdentity()
{
    return { 0.f, 0.f, 0.f, 1.f };
}

vec3 Core::Maths::Vector3RotateByQuaternion(vec3& v, const Quaternion& q)
{
    vec3 result;
    result.x = v.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + v.y * (2 * q.x * q.y - 2 * q.w * q.z) + v.z * (2 * q.x * q.z + 2 * q.w * q.y);
    result.y = v.x * (2 * q.w * q.z + 2 * q.x * q.y) + v.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + v.z * (-2 * q.w * q.x + 2 * q.y * q.z);
    result.z = v.x * (-2 * q.w * q.y + 2 * q.x * q.z) + v.y * (2 * q.w * q.x + 2 * q.y * q.z) + v.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);

    return result;
}

float Core::Maths::QuaternionLength(const Quaternion& q)
{
    float result = (float)sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    return result;
}

Quaternion Core::Maths::QuaternionNormalize(const Quaternion& q)
{
    Quaternion result = { 0.f, 0.f, 0.f, 0.f };

    float length, ilength;
    length = QuaternionLength(q);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;

    result.x = q.x * ilength;
    result.y = q.y * ilength;
    result.z = q.z * ilength;
    result.w = q.w * ilength;

    return result;
}

Quaternion Core::Maths::QuaternionFromAxisAngle(vec3 axis, float angle)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (mag(axis) != 0.0f)

        angle *= 0.5f;

    axis = normalize(axis);

    float sinres = sinf(angle);
    float cosres = cosf(angle);

    result.x = axis.x * sinres;
    result.y = axis.y * sinres;
    result.z = axis.z * sinres;
    result.w = cosres;

    result = QuaternionNormalize(result);

    return result;
}

Quaternion Core::Maths::QuaternionMultiply(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion result = { 0.f, 0.f, 0.f, 0.f };

    float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
    float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;

    result.x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    result.y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    result.z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    result.w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

    return result;
}

float Core::Maths::power(float base, float exp)
{
    if (exp == 0.f)
        return 1.f;

    int result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;

    return result;
}

mat4 Core::Maths::identity()
{
    return {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}

mat4 Core::Maths::translate(vec3 t)
{
    return {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        t.x, t.y, t.z, 1.f,
    };
}

mat4 Core::Maths::scale(float s)
{
    return {
        s, 0.f, 0.f, 0.f,
        0.f, s, 0.f, 0.f,
        0.f, 0.f, s, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}

mat4 Core::Maths::rotateX(float angleRadians)
{
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    return
    {
        1.f, 0.f, 0.f, 0.f,
        0.f,   c,   s, 0.f,
        0.f,  -s,   c, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}

mat4 Core::Maths::rotateY(float angleRadians)
{
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    return
    {
          c, 0.f,  -s, 0.f,
        0.f, 1.f, 0.f, 0.f,
          s, 0.f,   c, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}

mat4 Core::Maths::rotateZ(float angleRadians)
{
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    return
    {
          c,   s,  0.f, 0.f,
         -s,   c,  0.f, 0.f,
        0.f, 0.f,  1.f, 0.f,
        0.f, 0.f,  0.f, 1.f,
    };
}

mat4 Core::Maths::perspective(float fovY, float aspect, float near, float far, bool orthoView)
{
    float top = near * tan(fovY / 2.f);
    float right = top * aspect;
    if (orthoView)
    {
        return orthographic(-right, right, -top, top, near, far);
    }
    return frustum(-right, right, -top, top, near, far);
}

mat4 Core::Maths::orthographic(float left, float right, float bottom, float top, float near, float far)
{
    return
    {
        2 / (right - left),                 0.f,                               0.f,                         0.f,
        0.f,                                2 / (top - bottom),                0.f,                         0.f,
        0.f,                                0.f,                              -2 / (far - near),            0.f,
        -(right + left) / (right - left),   -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.f
    };
}

mat4 Core::Maths::frustum(float left, float right, float bottom, float top, float near, float far)
{
    return
    {
        (near * 2.f) / (right - left),   0.f,                              0.f,                               0.f,
        0.f,                             (near * 2.f) / (top - bottom),    0.f,                               0.f,
        (right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near),      -1.f,
        0.f,                             0.f,                             -(far * near * 2.f) / (far - near), 0.f
    };
}

bool Core::Maths::invert(const float m[16], float out[16])
{
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        out[i] = inv[i] * det;

    return true;
}
