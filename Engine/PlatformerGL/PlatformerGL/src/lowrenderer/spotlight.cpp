#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "lowrenderer/spotlight.hpp"
#include "core/debug/log.hpp"

using namespace LowRenderer;

SpotLight::SpotLight(const std::vector<float>& spotLightInfo)
{
    setCommonValues(spotLightInfo);

    position = { spotLightInfo[10], spotLightInfo[11], spotLightInfo[12] };
    direction = { spotLightInfo[13], spotLightInfo[14], spotLightInfo[15] };
    constant = spotLightInfo[16];
    linear = spotLightInfo[17];
    quadratic = spotLightInfo[18];
    cutoff = spotLightInfo[19];

}

SpotLight::SpotLight(const SpotLight& light)
{
    *this = light;
}

void    SpotLight::operator=(const SpotLight& light)
{
    Light::operator=(light);
    position = light.position;
    direction = light.direction;
    constant = light.constant;
    linear = light.linear;
    quadratic = light.quadratic;
    cutoff = light.cutoff;
}

void SpotLight::showImGuiControls()
{
    Light::showImGuiControls();
    ImGui::DragFloat3("pos", position.e);
    ImGui::DragFloat3("direction", direction.e);
    ImGui::SliderFloat("cut off", &cutoff, 0.1f, 14.f);
    ImGui::SliderFloat("constant", &constant, 0.f, 1.f);
    ImGui::SliderFloat("linear", &linear, 0.f, 1.f);
    ImGui::SliderFloat("quadratic", &quadratic, 0.f, 1.f);
    ImGui::NextColumn();
}