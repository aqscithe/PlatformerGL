#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "lowrenderer/directionallight.hpp"
#include "core/debug/log.hpp"

using namespace LowRenderer;

DirectionalLight::DirectionalLight(const std::vector<float>& dirLightInfo)
{
    setCommonValues(dirLightInfo);

    direction = { dirLightInfo[10], dirLightInfo[11], dirLightInfo[12] };
}

DirectionalLight::DirectionalLight(const DirectionalLight& light)
{
    *this = light;
}

void    DirectionalLight::operator=(const DirectionalLight& light)
{
    Light::operator=(light);
    direction = light.direction;
}

void DirectionalLight::showImGuiControls()
{
    Light::showImGuiControls();
    ImGui::DragFloat3("direction", direction.e);
    ImGui::NextColumn();
}