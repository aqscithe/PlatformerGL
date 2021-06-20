#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "lowrenderer/pointlight.hpp"
#include "core/debug/log.hpp"

using namespace LowRenderer;

PointLight::PointLight(const std::vector<float>& lightInfo)
{
    setCommonValues(lightInfo);

    position = { lightInfo[10], lightInfo[11], lightInfo[12] };
    constant = lightInfo[13];
    linear = lightInfo[14];
    quadratic = lightInfo[15];
}

PointLight::PointLight(const PointLight& light)
{
    *this = light;
}

void    PointLight::operator=(const PointLight& light)
{
    Light::operator=(light);
    position = light.position;
    constant = light.constant;
    linear = light.linear;
    quadratic = light.quadratic;
}

void PointLight::showImGuiControls()
{
    Light::showImGuiControls();
    ImGui::DragFloat3("pos", position.e);
    ImGui::SliderFloat("constant", &constant, 0.f, 1.f);
    ImGui::SliderFloat("linear", &linear, 0.f, 1.f);
    ImGui::SliderFloat("quadratic", &quadratic, 0.f, 1.f);
    ImGui::NextColumn();
}