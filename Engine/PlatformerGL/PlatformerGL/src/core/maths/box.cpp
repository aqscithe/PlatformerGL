#include "core/maths/box.hpp"

Core::Maths::Box::Box(const vec3& center, const vec3& extensions, const Quaternion& q)
	:center(center), extensions(extensions), q(q)
{
	setAttribs();
}

void Core::Maths::Box::setAttribs()
{
	triangles.push_back({  1.f * extensions.x / 2, -1.f * extensions.y / 2, -1.f * extensions.z / 2 });
	triangles.push_back({  1.f * extensions.x / 2, -1.f * extensions.y / 2,  1.f * extensions.z / 2 });
	triangles.push_back({ -1.f * extensions.x / 2, -1.f * extensions.y / 2,  1.f * extensions.z / 2 });
	triangles.push_back({ -1.f * extensions.x / 2, -1.f * extensions.y / 2, -1.f * extensions.z / 2 });
	triangles.push_back({  1.f * extensions.x / 2,  1.f * extensions.y / 2, -1.f * extensions.z / 2 });
	triangles.push_back({  1.f * extensions.x / 2,  1.f * extensions.y / 2,  1.f * extensions.z / 2	});
	triangles.push_back({ -1.f * extensions.x / 2,  1.f * extensions.y / 2,  1.f * extensions.z / 2 });
	triangles.push_back({ -1.f * extensions.x / 2,  1.f * extensions.y / 2, -1.f * extensions.z / 2 });

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(5);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(1);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(7);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(5);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(7);

	b = this;
}