#pragma once

#ifndef PI
	#define PI 3.14159265358979323846264338327950288f
#endif
#ifndef TORAD
	#define TORAD PI / 180.f
#endif
#ifndef TODEG
	#define TODEG 180.f / PI
#endif

#include <cmath>

namespace Core
{
	namespace Maths
	{
#pragma region Union Declarations

        union vec2
        {
            float   e[2];
            struct { float x; float y; };
        };

        union vec3
        {
            float   e[3];
            struct { float x; float y; float z; };
            struct { float r; float g; float b; };
            vec2 xy;
        };

        union vec4
        {
            vec4() = default;
            vec4(float x, float y, float z, float w)
                : x(x), y(y), z(z), w(w)
            {}

            vec4(vec3 xyz, float w)
                : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
            {}

            float e[4];
            struct { float x; float y; float z; float w; };
            struct { float r; float g; float b; float a; };
            vec3 rgb;
            vec3 xyz;
            vec2 xy;
        };

        typedef vec4 Quaternion;

        union mat4
        {
            float e[16];
            vec4 c[4];
        };

#pragma endregion

#pragma region vec3 
        inline vec3 operator+(vec3 a, vec3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
        inline vec3 operator+(vec3 a, float x) { return { a.x + x, a.y + x, a.z + x }; }

        inline vec3 operator+=(vec3& a, vec3 b)
        {
            a = a + b;
            return a;
        }

        inline vec3 operator-(vec3 a) { return { -a.x, -a.y, -a.z }; }
        inline vec3 operator-(vec3 a, vec3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }

        inline vec3 operator-=(vec3& a, vec3 b)
        {
            a = a - b;
            return a;
        }

        inline vec3 operator/(vec3 v, float w) { return { v.x / w, v.y / w, v.z / w }; }

        inline vec3 operator*(vec3 v, float k) { return { v.x * k, v.y * k, v.z * k }; }
        inline vec3 operator*(float k, vec3 v) { return { v.x * k, v.y * k, v.z * k }; }
        inline vec3 operator*(vec3 a, vec3 b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }

        inline vec3 operator*=(vec3& a, float k)
        {
            a = a * k;
            return a;
        }

        inline vec3 operator*=(vec3& a, vec3 b)
        {
            a = a * b;
            return a;
        }

#pragma endregion

#pragma region vec4

        inline vec4 operator*(const mat4& m, vec4 v)
        {
            vec4 r;
            r.x = v.x * m.c[0].e[0] + v.y * m.c[1].e[0] + v.z * m.c[2].e[0] + v.w * m.c[3].e[0];
            r.y = v.x * m.c[0].e[1] + v.y * m.c[1].e[1] + v.z * m.c[2].e[1] + v.w * m.c[3].e[1];
            r.z = v.x * m.c[0].e[2] + v.y * m.c[1].e[2] + v.z * m.c[2].e[2] + v.w * m.c[3].e[2];
            r.w = v.x * m.c[0].e[3] + v.y * m.c[1].e[3] + v.z * m.c[2].e[3] + v.w * m.c[3].e[3];
            return r;
        }
#pragma endregion   

#pragma region mat4

        mat4 identity();

        mat4 translate(vec3 t);
        mat4 rotateX(float pitch);
        mat4 rotateY(float yaw);
        mat4 rotateZ(float roll);
        mat4 scale(float s);

        mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
        mat4 perspective(float fovY, float aspect, float near, float far, bool ortho);
        mat4 frustum(float left, float right, float bottom, float top, float near, float far);

        inline mat4 operator*(const mat4& a, const mat4& b)
        {
            mat4 res = {};
            for (int c = 0; c < 4; ++c)
                for (int r = 0; r < 4; ++r)
                    for (int i = 0; i < 4; ++i)
                        res.c[c].e[r] += a.c[i].e[r] * b.c[c].e[i];
            return res;
        }

        inline mat4& operator*=(mat4& a, const mat4& b)
        {
            a = a * b;
            return a;
        }
#pragma endregion

#pragma region Constants & common math functions

        const float TAU = 6.283185307179586476925f;

        template<typename T>
        inline int min(T x, T y) { return x < y ? x : y; };
        template<typename T>
        inline int max(T x, T y) { return x > y ? x : y; };

        inline float cos(float x) { return cosf(x); }
        inline float sin(float x) { return sinf(x); }
        inline float tan(float x) { return tanf(x); }

        inline float max(float a, float b) { return a > b ? a : b; }
        inline float min(float a, float b) { return a < b ? a : b; }
        inline float clamp(float min, float max, float val) { return val < min ? min : val > max ? max : val; }

        inline float power(float base, float exp);

        bool invert(const float in[16], float out[16]);

#pragma endregion

		mat4	translateMatrix(const vec3& pos);

		mat4	rotateXMatrix(const float& rad);
		mat4	rotateYMatrix(const float& rad);
		mat4	rotateZMatrix(const float& rad);

		mat4	scaleMatrix(const vec3& scale);

		void	translate(vec3& dest, const vec3& t);
		vec3	pointVector(const vec3& a, const vec3& b);

		bool	operator!=(const vec3& a, const vec3& b);

		float	dot(const vec3& a, const vec3& b);

		vec3	vectProduct(const vec3& a, const vec3& b);

		float	mag(const vec3& v);
		float	sqrMag(const vec3& v);
		vec3	normalize(const vec3& v);

        void    sortAscendingFloat(float* v, const int num, int* order);

        Quaternion  QuaternionIdentity();
        vec3        Vector3RotateByQuaternion(vec3& v, const Quaternion& q);
        float       QuaternionLength(const Quaternion& q);
        Quaternion  QuaternionNormalize(const Quaternion& q);
        Quaternion  QuaternionFromAxisAngle(vec3 axis, float angle);
        Quaternion  QuaternionMultiply(const Quaternion& q1, const Quaternion& q2);
	}
}