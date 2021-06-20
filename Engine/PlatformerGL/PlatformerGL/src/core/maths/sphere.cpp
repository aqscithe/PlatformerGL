#include "core/maths/sphere.hpp"

Core::Maths::Sphere::Sphere(const vec3& omega, const float& radius)
	:omega(omega), radius(radius)
{
	setAttribs(50, 25);
}

void Core::Maths::Sphere::setAttribs()
{
    setAttribs(50, 25);
}

void Core::Maths::Sphere::setAttribs(const int lon, const int lat)
{
    float r = radius;

    float angleLon = 2.f * PI / lon;
    float angleLat = PI / lat;

    for (int i = 0; i < lat; ++i)
    {
        for (int j = 0; j < lon; ++j)
        {
            triangles.push_back({ r * cosf(j * angleLon) * sinf(i * angleLat),
                                  r * cosf(i * angleLat),
                                  r * sinf(j * angleLon) * sinf(i * angleLat) });
        }
    }

    for (int i = 0; i < triangles.size() - lon; ++i)
    {
        // disposition of vertex
        // i            i + 1
        // i + lon      i + lon + 1

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + lon);

        if (i == triangles.size() - lon - 1)
            continue;

        indices.push_back(i + 1);
        indices.push_back(i + lon);
        indices.push_back(i + lon + 1);
    }

    sph = this;
}