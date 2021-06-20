#pragma once

#include "light.hpp"

#include "core/maths/maths.hpp"

namespace LowRenderer
{
    class SpotLight : public Light
    {
    public:
        SpotLight() = default;
        SpotLight(const std::vector<float> & dirLightInfo);
        SpotLight(const SpotLight& light);

        void                operator=(const SpotLight& light);

        void                showImGuiControls() override;

        Core::Maths::vec3   position{ 3.f, 0.3f, 0.35f };
        Core::Maths::vec3   direction{ -1.f, 0.f, 0.f };

        float               cutoff = 6.f;
        float               constant = 0.f;
        float               linear = 1.f;
        float               quadratic = 0.f;

        
    };
}