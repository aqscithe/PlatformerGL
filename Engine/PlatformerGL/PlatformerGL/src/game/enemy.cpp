#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "game/enemy.hpp"

using namespace Game;

Enemy::Enemy(
	const std::vector<float>& transformInfo, const std::string& colliderName, const std::vector<float>& colliderAttribs, 
	const std::vector<int>& gameObjAttrib, const std::string& _customTexture
) 
	: Entity(transformInfo)
{
	tag = Tag::ENEMY;
	damage = gameObjAttrib[0];
	target = Core::Maths::vec3{ 0.f, 0.f, 0.f };

	setCollider(colliderName, colliderAttribs);
}


void	Enemy::showImGuiControls()
{
	ImGui::NextColumn();
	ImGui::Checkbox("enabled", &model.enabled);
	ImGui::Checkbox("texture enabled", &model.textureEnabled);
	if (!model.textureEnabled)
		ImGui::ColorEdit3("model color", model.gfxColor.e);
	ImGui::Checkbox("wireframe", &model.wireframe);

	std::string tagLabel = "ENEMY";
	ImGui::LabelText("Tag", tagLabel.c_str());

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
		ImGui::SliderInt("Damage", &damage, 0, 2);
	}
	ImGui::NextColumn();
}
