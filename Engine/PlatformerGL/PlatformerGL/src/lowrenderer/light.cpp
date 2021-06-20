#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "lowrenderer/light.hpp"
#include "core/debug/log.hpp"

using namespace LowRenderer;

Light::Light(const std::vector<float>& lightInfo)
{
    setCommonValues(lightInfo);
}

void LowRenderer::Light::setCommonValues(const std::vector<float>& lightInfo)
{
    enabled = int(lightInfo[0]);
    ambient = { lightInfo[1], lightInfo[2], lightInfo[3] };
    diffuse = { lightInfo[4], lightInfo[5], lightInfo[6] };
    specular = { lightInfo[7], lightInfo[8], lightInfo[9] };
}

Light::Light(const Light& light)
{
    *this = light;
}

void Light::operator=(const Light& light)
{
    enabled = light.enabled;
    ambient = light.ambient;
    diffuse = light.diffuse;
    specular = light.specular;
}

void Light::showImGuiControls()
{
    ImGui::NextColumn();
    ImGui::Checkbox("enabled ", &enabled);
    ImGui::ColorEdit3("ambient", ambient.e);
    ImGui::ColorEdit3("diffuse", diffuse.e);
    ImGui::ColorEdit3("specular", specular.e);
    
}