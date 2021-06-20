#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "game/gameobject.hpp"
#include "core/core.hpp"
#include "core/debug/log.hpp"
#include "core/maths/box.hpp"
#include "core/maths/sphere.hpp"
#include "core/debug/assertion.hpp"

using namespace Game;
using namespace Core::Maths;

GameObject::GameObject(const std::vector<float>& _transform, const std::string& colliderName, const std::vector<float>& colliderAttribs) 
{
    transform.position = vec3{ _transform[0], _transform[1], _transform[2] };
    transform.rotation = vec3{ _transform[3], _transform[4], _transform[5] };
    transform.scale = vec3{ _transform[6], _transform[7], _transform[8] };

    setCollider(colliderName, colliderAttribs);
}

GameObject::GameObject(const GameObject& other)
{
    *this = other;
}

GameObject::~GameObject()
{
    //if the scene is destroying, then this gameobject should be destroyed
    if (!destroying)
        return;

    //has a primitive shape
    if (!shape)
        return;

    if (shape->b)               //has a box
    {
        delete shape->b;
        shape->setPrimPointersToNull();
        return;
    }
    if (shape->sph)             //has a sphere
    {
        delete shape->sph;
        shape->setPrimPointersToNull();
        return;
    }
}

void GameObject::setCollider(const std::string& colliderName, const std::vector<float>& colliderAttribs)
{
    if (colliderName == "cube")
    {
        vec3 center = { colliderAttribs[0], colliderAttribs[1], colliderAttribs[2] };
        vec3 extensions = { colliderAttribs[3], colliderAttribs[4], colliderAttribs[5] };
        shape = new Box(center, extensions, QuaternionIdentity());
        shape->collider = Core::Maths::Collider::BOX;
    }
    else if (colliderName == "sphere")
    {
        vec3 omega = { colliderAttribs[0], colliderAttribs[1], colliderAttribs[2] };
        float radius = colliderAttribs[3];
        shape = new Sphere(omega, radius);
        shape->collider = Core::Maths::Collider::SPHERE;
    }
}

void    GameObject::operator=(const GameObject& other)
{
    transform = other.transform;
    shape = other.shape;
    model = other.model;
    tag = other.tag;
    selected = other.selected;
    customTexture = other.customTexture;
    destroying = other.destroying;
    hasPrimShape = other.hasPrimShape;

}

void    GameObject::addShape(Core::Maths::Primitives* shape)
{
    this->shape = shape;
}

void    GameObject::addShader(const std::vector<std::string>& modelShaders, const Core::Maths::vec3& lightCounts)
{
    model.gfxShader = Resources::Shader(modelShaders[0], modelShaders[1], lightCounts);
    model.colliderShader = Resources::Shader(modelShaders[2], modelShaders[3]);
}

void    GameObject::fillMesh(const std::vector<Resources::Mesh>& meshes)
{
    model.meshes = meshes;
}

void    GameObject::addMesh(const std::string& resourceInfo)
{
    model.meshes.push_back(Resources::Mesh(resourceInfo, transform));
}

void    GameObject::addMesh()
{
    model.meshes.push_back(Resources::Mesh(transform));
}

void    GameObject::loadTextures(const std::vector<std::string>& materials, const std::vector<std::string>& texFiles,
            std::map<std::string, unsigned int>& cachedTextures)
{

    if (customTexture != "None" && model.meshes.size() == 2)
    {
        if (cachedTextures.count(customTexture) > 0)
        {
            model.meshes[0].texture.texCount = cachedTextures[customTexture];
        }
        else
        {
            cachedTextures.emplace(customTexture, model.meshes[0].texture.bindTexture());
            model.meshes[0].texture.processTexData(customTexture);
        }
            
        for (int i = 0; i < materials.size(); ++i)
        {
            if (model.meshes[1].materialsInfo == materials.at(i))
            {
                model.meshes[1].texture.bindTexture();
                Core::Debug::Assertion::assertTest(!texFiles[i].empty());
                if (cachedTextures.count(materials[i]) > 0)
                    model.meshes[1].texture.texCount = cachedTextures[texFiles[i]];
                else
                {
                    cachedTextures.emplace(texFiles[i], model.meshes[1].texture.bindTexture());
                    model.meshes[1].texture.processTexData(texFiles[i]);
                }
            }
        }
    }
    else
    {
        for (Resources::Mesh& mesh : model.meshes)
        {
            for (int i = 0; i < materials.size(); ++i)
            {
                if (mesh.materialsInfo == materials.at(i))
                {
                    mesh.texture.bindTexture();
                    Core::Debug::Assertion::assertTest(!texFiles[i].empty());
                    if (cachedTextures.count(materials[i]) > 0)
                        mesh.texture.texCount = cachedTextures[texFiles[i]];
                    else
                    {
                        cachedTextures.emplace(texFiles[i], mesh.texture.bindTexture());
                        mesh.texture.processTexData(texFiles[i]);
                    }
                }
            }
        }
    }
    
}

void    GameObject::defineVAO()
{
    for (int i = 0; i < model.meshes.size(); ++i)
    {
        model.meshes[i].defineVAO();
    }
}

void GameObject::showImGuiControls()
{
    ImGui::NextColumn();
    ImGui::Checkbox("enabled", &model.enabled);
    ImGui::Checkbox("texture enabled", &model.textureEnabled);
    if (!model.textureEnabled)
        ImGui::ColorEdit3("model color", model.gfxColor.e);
    ImGui::Checkbox("wireframe", &model.wireframe);

    const char* tagLabel;
    switch (tag)
    {
    case Tag::PLAYER:
        tagLabel = "PLAYER";
        break;
    case Tag::ENEMY:
        tagLabel = "ENEMY";
        break;
    case Tag::PLATFORM:
        tagLabel = "PLATFORM";
        break;
    case Tag::UNASSIGNED:
        tagLabel = "UNASSIGNED";
        break;
    default:
        tagLabel = "NO TAG";
        std::string statement = "Invalid TAG: " + static_cast<int>(tag);
        Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
        break;
    }
    ImGui::LabelText("Tag", tagLabel);

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("position", transform.position.e);
        ImGui::DragFloat3("rotation", transform.rotation.e);
        ImGui::SliderFloat3("scale", transform.scale.e, 0.01f, 5.f);
    }

    if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Visible", &model.colliderVisible);
        ImGui::ColorEdit3("Color", model.colliderColor.e);
    }
    ImGui::NextColumn();
}

