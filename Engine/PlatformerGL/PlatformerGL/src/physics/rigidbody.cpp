#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "physics/rigidbody.hpp"
#include "time.hpp"

#include <iostream>

using namespace Physics;

RigidBody::RigidBody(Core::Maths::vec3& position)
{
	velocity = Core::Maths::vec3{ 0.f, 0.f, 0.f };
	acceleration = Core::Maths::vec3{ 0.f, gravity, 0.f };
	mass = 1.f;
}

void RigidBody::applyForce(const Core::Maths::vec3& force)
{
	float fixedDeltaTime = Time::fixedDeltaTime();
	acceleration += force / mass;
	velocity += acceleration * fixedDeltaTime;
}

void RigidBody::applyVelocity(vec3& v, const float& resistance)
{
	float fixedDeltaTime = Time::fixedDeltaTime();

	//std::cout << normal.x << ", " << normal.y << ", " << normal.z << std::endl;

	if (v.x > 0.f)
	{
		if (v.x - v.x * resistance * fixedDeltaTime > 0.f)
			v.x -= v.x * resistance * fixedDeltaTime;
		else
			v.x = 0.f;
	}
	else if (v.x < 0.f)
	{
		if (v.x - v.x * resistance * fixedDeltaTime < 0.f)
			v.x -= v.x * resistance * fixedDeltaTime;
		else
			v.x = 0.f;
	}

	if (v.z > 0.f)
	{
		if (v.z - v.z * resistance * fixedDeltaTime > 0.f)
			v.z -= v.z * resistance * fixedDeltaTime;
		else
			v.z = 0.f;
	}
	else if (v.z < 0.f)
	{
		if (v.z - v.z * resistance * fixedDeltaTime < 0.f)
			v.z -= v.z * resistance * fixedDeltaTime;
		else
			v.z = 0.f;
	}

	velocity = v;
}

void RigidBody::resetAcceleration()
{
	acceleration = { 0.f, gravity, 0.f };
}

void RigidBody::supportReaction(const vec3& normal)
{
	velocity -= dot(velocity, normal) * normal;
}

void Physics::RigidBody::update(const vec3& position)
{
	float fixedDeltaTime = Time::fixedDeltaTime();

	vec3 nextVelocity = velocity + acceleration * fixedDeltaTime;
	vec3 nextPoint = nextVelocity * fixedDeltaTime;

	Segment t(position, position + nextPoint);

	tn = t;
}

void RigidBody::showImGuiControls()
{
	ImGui::DragFloat("gravity", &gravity);
	ImGui::DragFloat("mass", &mass);
	ImGui::DragFloat3("velocity", velocity.e);
	ImGui::DragFloat3("acceleration", acceleration.e);
	ImGui::DragFloat("friction", &friction);
	ImGui::DragFloat("air resistance", &airResistance);
}