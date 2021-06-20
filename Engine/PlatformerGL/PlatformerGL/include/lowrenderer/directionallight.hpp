#pragma once

#include "lowrenderer/light.hpp"

namespace LowRenderer
{
    class DirectionalLight : public Light
    {
    public:
        DirectionalLight() = default;
        DirectionalLight(const std::vector<float>& dirLightInfo);
        DirectionalLight(const DirectionalLight& light);

        void                operator=(const DirectionalLight& light);

        void                showImGuiControls() override;

        Core::Maths::vec3   direction{ -1.f, 0.f, 0.f };
    };
}