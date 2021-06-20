#pragma once

#include "lowrenderer/light.hpp"

namespace LowRenderer
{
    class PointLight : public Light
    {
    public:
        PointLight() = default;
        PointLight(const std::vector<float> & dirLightInfo);
        PointLight(const PointLight& light);

        void                operator=(const PointLight& light);

        void                showImGuiControls() override;

        Core::Maths::vec3   position{ 3.f, 0.3f, 0.35f };

        float               constant = 0.f;
        float               linear = 1.f;
        float               quadratic = 0.f;

        
    };
}