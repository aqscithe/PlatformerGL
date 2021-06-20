#include "lowrenderer/model.hpp"


using namespace LowRenderer;
using namespace Core::Maths;

Model::Model(const Model& other)
{
    *this = other;
}

void Model::operator=(const Model& other)
{
    gfxShader = other.gfxShader;
    colliderShader = other.colliderShader;

    meshes = other.meshes;

    materialsFile = other.materialsFile;
    name = other.name;

    gfxColor = other.gfxColor;
    colliderColor = other.colliderColor;

    colliderVisible = other.colliderVisible;
    textureEnabled = other.textureEnabled;
    wireframe = other.wireframe;
    enabled = other.enabled;
    currMesh = other.currMesh;
}

void    Model::useShader()
{
    glUseProgram(gfxShader.shaderProgram);
}

void    Model::setShader(const Core::Maths::mat4& mvp, const Core::Maths::mat4& modelMat4, const Core::Maths::vec3& camPos, bool outline)
{
    useShader();
    setShaderAttrib(mvp, modelMat4, camPos, outline);
}

void    Model::setColliderShader(const Core::Maths::mat4& mvp)
{
    glUseProgram(colliderShader.shaderProgram);
    colliderShader.setMat4("mvp", mvp);
    colliderShader.setVec3("colliderColor", colliderColor);
    
}

void LowRenderer::Model::setShaderAttrib(const Core::Maths::mat4& mvp, const Core::Maths::mat4& modelMat4, const Core::Maths::vec3& camPos, bool outline)
{
    gfxShader.setMat4("mvp", mvp);
    gfxShader.setMat4("modelMat4", modelMat4);
    gfxShader.setBool("textureEnabled", textureEnabled);
    gfxShader.setVec3("modelColor", gfxColor);
    gfxShader.setVec3("camPos", camPos);
    gfxShader.setBool("outline", outline);
}

void    LowRenderer::Model::setLights(
    const std::vector<LowRenderer::SpotLight>& spotLights, const std::vector<LowRenderer::DirectionalLight>& dirLights, 
    const std::vector<LowRenderer::PointLight>& pointLights
)
{
    setDirectionalLights(dirLights);
    setPointLights(pointLights);
    setSpotLights(spotLights);
}

void Model::setSpotLights(const std::vector<LowRenderer::SpotLight>& spotLights)
{
    int i = 0;
    for (auto& spotLight : spotLights)
    {
        std::string lightLabel = "spotLights[" + std::to_string(i) + "].";
        gfxShader.setBool(lightLabel + "enabled", spotLight.enabled);
        gfxShader.setVec3(lightLabel + "position", spotLight.position);
        gfxShader.setVec3(lightLabel + "direction", spotLight.direction);
        gfxShader.setFloat(lightLabel + "cutoff", spotLight.cutoff);
        gfxShader.setFloat(lightLabel + "constant", spotLight.constant);
        gfxShader.setFloat(lightLabel + "linear", spotLight.linear);
        gfxShader.setFloat(lightLabel + "quadratic", spotLight.quadratic);
        gfxShader.setVec3(lightLabel + "ambient", spotLight.ambient);
        gfxShader.setVec3(lightLabel + "diffuse", spotLight.diffuse);
        gfxShader.setVec3(lightLabel + "specular", spotLight.specular);
        ++i;
    }
}

void Model::setDirectionalLights(const std::vector<LowRenderer::DirectionalLight>& dirLights)
{
    int i = 0;
    for (auto& dirLight : dirLights)
    {
        std::string lightLabel = "dirLights[" + std::to_string(i) + "].";
        gfxShader.setBool(lightLabel + "enabled", dirLight.enabled);
        gfxShader.setVec3(lightLabel + "direction", dirLight.direction);
        gfxShader.setVec3(lightLabel + "ambient", dirLight.ambient);
        gfxShader.setVec3(lightLabel + "diffuse", dirLight.diffuse);
        gfxShader.setVec3(lightLabel + "specular", dirLight.specular);
        ++i;
    }
}

void Model::setPointLights(const std::vector<LowRenderer::PointLight>& pointLights)
{
    int i = 0;
    for (auto& pointLight : pointLights)
    {
        std::string lightLabel = "pointLights[" + std::to_string(i) + "].";
        gfxShader.setBool(lightLabel + "enabled", pointLight.enabled);
        gfxShader.setVec3(lightLabel + "position", pointLight.position);
        gfxShader.setFloat(lightLabel + "constant", pointLight.constant);
        gfxShader.setFloat(lightLabel + "linear", pointLight.linear);
        gfxShader.setFloat(lightLabel + "quadratic", pointLight.quadratic);
        gfxShader.setVec3(lightLabel + "ambient", pointLight.ambient);
        gfxShader.setVec3(lightLabel + "diffuse", pointLight.diffuse);
        gfxShader.setVec3(lightLabel + "specular", pointLight.specular);
        ++i;
    }
}

