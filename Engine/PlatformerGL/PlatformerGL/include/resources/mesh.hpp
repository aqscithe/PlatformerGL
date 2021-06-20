#pragma once

#include <string>
#include <vector>
#include <string>

#include <glad/glad.h>

#include "core/core.hpp"
#include "core/maths/maths.hpp"
#include "resources/texture.hpp"
#include "physics/transform.hpp"

namespace Resources
{
    enum class FaceType
    {
        TRIANGLE,
        QUAD
    };

    class Mesh
    {
    public:
        Mesh(const std::string materialsInfo, const Physics::Transform& modelTransform);
        Mesh(const Physics::Transform& modelTransform);
        Mesh(const Mesh& other);

        void                            operator=(const Mesh& other);

        void	                        setIndices();
        void                            defineVAO();

        FaceType                        faceType = FaceType::TRIANGLE;
        
        Resources::Texture              texture;
        std::vector<Core::rdrVertex>    rdrVertices;
        std::vector<unsigned int>       indices;

        Physics::Transform              worldTransform;
        std::string	                    materialsInfo;

        GLuint                          VAO = 0;
        GLuint                          EBO = 0;

    private:
        void                            processVAO();
        void                            setAttributes();
    };
}