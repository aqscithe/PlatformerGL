#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "game/platform.hpp"
#include "core/core.hpp"
#include "core/maths/box.hpp"
#include "core/debug/assertion.hpp"

using namespace Game;
using namespace Core::Maths;

Platform::Platform(
	const std::vector<float>& transformInfo, const std::string& colliderName, const std::vector<float>& colliderAttribs, 
	const std::vector<int>& gameObjAttrib, const std::string& _customTexture
)
	: GameObject(transformInfo, colliderName, colliderAttribs)
{
	tag = Tag::PLATFORM;
	customTexture = _customTexture;
}
