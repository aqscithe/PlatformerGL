#pragma once

#include <vector>
#include <string>

#include "resources/mesh.hpp"
#include "resources/texture.hpp"
#include "resources/shader.hpp"
#include "lowrenderer/directionallight.hpp"
#include "lowrenderer/spotlight.hpp"
#include "lowrenderer/pointlight.hpp"

namespace LowRenderer
{
    class Model
    {
    public:
        Model() = default;
        Model(const Model& other);

        void                                operator=(const Model& other);

        void                                setColliderShader(const Core::Maths::mat4& mvp);
        void                                setShader(const Core::Maths::mat4& mvp, const Core::Maths::mat4& modelMat4, const Core::Maths::vec3& camPos, bool outline);
        void                                setLights(
                                                const std::vector<LowRenderer::SpotLight>& spot, const std::vector<LowRenderer::DirectionalLight>& dirLights,
                                                const std::vector<LowRenderer::PointLight>& point
                                            );

        Core::Maths::vec3                   gfxColor{ 1.f, 1.f, 1.f };
        Core::Maths::vec3                   colliderColor{ 0.f, 1.f, 0.f};

        Resources::Shader                   gfxShader;
        Resources::Shader                   colliderShader;

        std::vector<Resources::Mesh>		meshes;

        std::string							materialsFile;
        std::string                         name;

        bool                                colliderVisible = false;
        bool                                textureEnabled = true;
        bool                                wireframe = false;
        bool                                enabled = true;

    private:
        void                                useShader();
        void                                setShaderAttrib(const Core::Maths::mat4& mvp, const Core::Maths::mat4& modelMat4, const Core::Maths::vec3& camPos, bool outline);
        void								setDirectionalLights(const std::vector<LowRenderer::DirectionalLight>& dirLights);
        void								setPointLights(const std::vector<LowRenderer::PointLight>& pointLights);
        void								setSpotLights(const std::vector<LowRenderer::SpotLight>& spotLights);

        int                                 currMesh = 0;
    };
}