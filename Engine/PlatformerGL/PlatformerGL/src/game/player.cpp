#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "game/player.hpp"
#include "core/debug/log.hpp"
#include "core/maths/box.hpp"
#include "physics/collision/collision.hpp"
#include "time.hpp"

#include <cmath>
#include <iostream>

using namespace Game;
using namespace Core::Debug;
using namespace Core::Maths;

Player::Player(
	const std::vector<float>& transformInfo, const std::string& colliderName, const std::vector<float>& colliderAttribs, 
	const std::vector<int>& gameObjAttrib, const std::string& _customTexture
)
	: Entity(transformInfo)
{
	tag = Tag::PLAYER;
	state = State::GROUNDED;
	maxHealth = gameObjAttrib[0];
	health = maxHealth;
	speed = 3;
	customTexture = _customTexture;
	setCollider(colliderName, colliderAttribs);
}

void Player::update(const Input& inputs, std::vector<GameObject*>& gos)
{
	float fixedDeltaTime = Time::fixedDeltaTime();

	for (int i = Time::fixing(); i > 0; --i)
	{
		calcHoriTranslation(inputs, gos);
		rigidBody.update(transform.position);

		bool initiateJump = false;
		switch (state)
		{
			case State::GROUNDED :
				initiateJump = jump(inputs);
				break;
			case State::JUMPING :
				verticalMove(initiateJump);
				rigidBody.resetAcceleration();
				break;
			default:
				std::string statement = "Invalid player state - Player::update: " + static_cast<int>(state);
				Log::print(statement, LogType::ERROR);
				break;
		}

		vec3 normal = { 0.f, 0.f, 0.f };
		processCollisions(gos, normal, initiateJump);

		horizontalMove(normal);
	}
}

void Player::processCollisions(std::vector<GameObject*>& gos, vec3& normal, const bool initiateJump)
{
	bool collisionFound = false;
	if (!initiateJump)
	{
		vec3 intersection = { 0.f, 0.f, 0.f };
		for (auto& go : gos)
		{
			if (collisionFound)
				break;

			switch (go->shape->collider)
			{
			case Collider::BOX:
			{
				RoundedBox rb(go->shape->b->center,
							{ go->shape->b->extensions.x + shape->sph->radius * 2.f,
							  go->shape->b->extensions.y + shape->sph->radius * 2.f,
							  go->shape->b->extensions.z + shape->sph->radius * 2.f,
							  shape->sph->radius },
							  go->shape->b->q);
				Box b(go->shape->b->center, go->shape->b->extensions + shape->sph->radius * 2.f, go->shape->b->q);

				//radius * 2 because there is same gap both sides of the quad
				if (Physics::Collision::cSegmentBox(rigidBody.tn, b, intersection, normal))
				{
					//don't need the intersection

					groundPlayer(normal);
					collisionFound = true;
				}
				else
				{
					state = State::JUMPING;
					normal = { 0.f, 0.f, 0.f };
				}

				break;
			}

			case Collider::SPHERE:
				break;

			default:
				break;
			}
		}
	}

	if (!collisionFound)
		calcVertTranslation();
}

void Player::calcHoriTranslation(const Input& inputs, std::vector<GameObject*>& gos)
{
	float fixedDeltaTime = Time::fixedDeltaTime();
	vec3 netVelocity = rigidBody.getVelocity();

	calcHoriNetVelocity(inputs, netVelocity);

	switch (state)
	{
		case State::GROUNDED :
			rigidBody.applyVelocity(netVelocity, rigidBody.getFriction());
			break;

		case State::JUMPING :
			rigidBody.applyVelocity(netVelocity, rigidBody.getAirResistance());
			break;

		default:
			std::string statement = "Invalid player state - Player::update: " + static_cast<int>(state);
			Log::print(statement, LogType::ERROR);
			break;
	}
}

void Player::calcHoriNetVelocity(const Input& inputs, vec3& netVelocity)
{
	if (inputs.moveForward)
	{
		if (netVelocity.z - speed < -speed)
			netVelocity.z = -speed;
		else
			netVelocity.z -= speed;
	}

	if (inputs.moveBackward)
	{
		if (netVelocity.z + speed > speed)
			netVelocity.z = speed;
		else
			netVelocity.z += speed;
	}

	if (inputs.moveLeft)
	{
		if (netVelocity.x - speed < -speed)
			netVelocity.x = -speed;
		else
			netVelocity.x -= speed;
	}

	if (inputs.moveRight)
	{
		if (netVelocity.x + speed > speed)
			netVelocity.x = speed;
		else
			netVelocity.x += speed;
	}
}

void Player::verticalMove(bool& initiateJump)
{
	if (jumpForce > 0.f)
		initiateJump = true;

	auto netForce = vec3{ 0.f, jumpForce, 0.f };
	rigidBody.applyForce(netForce);
	deteriorateJumpForce();
}

void Player::horizontalMove(const vec3& normal)
{
	float fixedDeltaTime = Time::fixedDeltaTime();

	if (normal.x == 0)
		transform.position.x += rigidBody.getVelocity().x * fixedDeltaTime;
	if (normal.z == 0)
		transform.position.z += rigidBody.getVelocity().z * fixedDeltaTime;

	updateColliderPos();
}

void Player::calcVertTranslation()
{
	float fixedDeltaTime = Time::fixedDeltaTime();

	transform.position.y += rigidBody.getVelocity().y * fixedDeltaTime;

	updateColliderPos();
}

void Player::groundPlayer(const vec3& normal)
{
	jumpForce = 0.f;
	state = State::GROUNDED;

	rigidBody.supportReaction(normal);
	updateColliderPos();
}

void Player::updateColliderPos()
{
	switch (shape->collider)
	{
		case Collider::SPHERE :
			shape->sph->omega = transform.position;
			break;
		case Collider::BOX :
			shape->b->center = transform.position;
			break;
		default:
			std::string statement = "invalid collider for player " + static_cast<int>(shape->collider);
			Log::print(statement, LogType::ERROR);
			break;
	}
}

void Player::deteriorateJumpForce()
{
	if (jumpForce + G < 0.f)
		jumpForce = 0.f;
	else
		jumpForce += G;
}

bool Player::jump(const Input& inputs)
{
	if (inputs.jump)
	{
		jumpForce = initialJumpForce;
		state = State::JUMPING;

		return true;
	}

	return false;
}

void Player::heal(const int h)
{
	if (health + h > maxHealth)
		health = maxHealth;
	else
		health += h;
}

void Player::takeDamage(const int damage)
{
	if (health - damage < 0)
		health = 0;
	else
		health -= damage;
}

void Player::showImGuiControls()
{
	ImGui::NextColumn();
	ImGui::Checkbox("enabled", &model.enabled);
	ImGui::Checkbox("texture enabled", &model.textureEnabled);
	if (!model.textureEnabled)
		ImGui::ColorEdit3("model color", model.gfxColor.e);

	ImGui::Checkbox("wireframe", &model.wireframe);
	const char* tagLabel;
	switch (tag)
	{
		case Tag::PLAYER :
			tagLabel = "PLAYER";
				break;
		case Tag::ENEMY :
				tagLabel = "ENEMY";
				break;
		case Tag::PLATFORM:
			tagLabel = "PLATFORM";
			break;
		case Tag::UNASSIGNED:
			tagLabel = "UNASSIGNED";
			break;
		default:
			tagLabel = "NO TAG";
			std::string statement = "Invalid TAG: " + static_cast<int>(tag);
			Log::print(statement, LogType::ERROR);
			break;
	}
	ImGui::LabelText("Tag", tagLabel);

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("position", transform.position.e);
		ImGui::DragFloat3("rotation", transform.rotation.e);
		ImGui::SliderFloat3("scale", transform.scale.e, 0.01f, 5.f);
	}

	if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
	{
		rigidBody.showImGuiControls();
	}

	if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Visible", &model.colliderVisible);
		ImGui::ColorEdit3("Color", model.colliderColor.e);
	}

	if (ImGui::CollapsingHeader("Attributes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string stateLabel = "";
		switch (state)
		{
			case State::GROUNDED :
				stateLabel = "GROUNDED";
				break;
			case State::JUMPING :
				stateLabel = "JUMPING";
				break;
			default:
				stateLabel = "NO STATE";
				std::string statement = "Invalid player state: " + static_cast<int>(state);
				Log::print(statement, LogType::ERROR);
				break;
		}
		ImGui::LabelText("State", stateLabel.c_str());
		ImGui::SliderInt("Health", &health, 0, 10);
		ImGui::DragFloat("InitialJumpForce", &initialJumpForce);
		ImGui::DragFloat("JumpForce", &jumpForce);
		ImGui::DragFloat("Speed", &speed);
		ImGui::NextColumn();
	}
}

int Player::getHealth() const
{
	return health;
}