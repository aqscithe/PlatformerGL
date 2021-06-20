
#include "resources/mesh.hpp"
#include "core/core.hpp"

using namespace Resources;
using namespace Core::Maths;

Mesh::Mesh(const std::string materialsInfo, const Physics::Transform& modelTransform) : materialsInfo(materialsInfo), worldTransform(modelTransform)
{
    

}

Mesh::Mesh(const Physics::Transform& modelTransform) : worldTransform(modelTransform)
{
    
}

Mesh::Mesh(const Mesh& other)
{
    *this = other;
}

void Mesh::operator=(const Mesh& other)
{
    materialsInfo = other.materialsInfo;
    rdrVertices = other.rdrVertices;
    indices = other.indices;
    texture = other.texture;

    worldTransform = other.worldTransform;
    faceType = other.faceType;
    VAO = other.VAO;
    EBO = other.EBO;
}

void Mesh::setIndices()
{
    int vertexCount = rdrVertices.size();
    for (int i = 0; i < vertexCount; ++i)
        indices.push_back(i);
}

void    Mesh::defineVAO()
{
    processVAO();
    setAttributes();
}

void Mesh::processVAO()
{
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(rdrVertices.front()) * rdrVertices.size(),
        rdrVertices.data(),
        GL_STATIC_DRAW
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices.front()) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW
    );
}

void Resources::Mesh::setAttributes()
{
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coordinate attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);
}