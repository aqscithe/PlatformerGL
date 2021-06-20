#pragma once

#include <vector>

#include "core/maths/maths.hpp"

namespace LowRenderer
{
    class Light
    {
    public:
        Light() = default;
        Light(const std::vector<float>& lightInfo);
        
        Light(const Light& light);

        void                operator=(const Light& light);

        virtual void        setCommonValues(const std::vector<float>& lightInfo);
        virtual void        showImGuiControls();

        bool                enabled = false;

        Core::Maths::vec3   ambient{ 1.0f, 1.0f, 1.0f };
        Core::Maths::vec3   diffuse{ 0.1f, 0.4f, 0.7f };
        Core::Maths::vec3   specular{ 1.0f, 0.4f, 0.5f };
    };
}