#include <iostream>
#include <fstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "core/debug/log.hpp"
#include "resources/scene.hpp"
#include "game/enemy.hpp"
#include "game/player.hpp"

using namespace Resources;
using namespace Game;

Scene::Scene(const std::string& scnName) : name(scnName)
{}

Scene::~Scene()
{
    std::cout << "destroying scene" << std::endl;

    for (auto& go : gameObjects)
    {
        go->destroying = true;
    }

    for (auto& p : players)
    {
        p.destroying = true;
    }

    for (auto& e : enemies)
    {
        e.destroying = true;
    }

    for (auto& pl : platforms)
    {
        pl.destroying = true;
    }
}


void	Scene::addGameObject(const std::vector<float>& modelAttribs, const std::string& colliderName, const std::vector<float>& colliderAttribs, 
            const std::vector<int>& gameObjAttribs, const std::string& customTexture)
{
    std::vector<float> transform = std::vector<float>(modelAttribs.begin(), modelAttribs.begin() + (modelAttribs.size() - 1));
    int tag = int(modelAttribs.back());
    switch (tag)
    {
        case static_cast<int>(Tag::PLAYER) :
            players.push_back( Player(transform, colliderName, colliderAttribs, gameObjAttribs, customTexture));
            break;
        
        case static_cast<int>(Tag::ENEMY) :
            enemies.push_back(Enemy(transform, colliderName, colliderAttribs, gameObjAttribs, customTexture));
            break;
        case static_cast<int>(Tag::PLATFORM) :
            platforms.push_back(Platform(transform, colliderName, colliderAttribs, gameObjAttribs, customTexture));
            break;
        default:
            std::string statement = "invalid tag code: " + tag;
            statement += " | cannot create gameobject";
            Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
            break;
    }
}


void Resources::Scene::setGameObjects()
{
    for (auto& enemy : enemies)
        gameObjects.push_back(&enemy);
    for (auto& platform : platforms)
        gameObjects.push_back(&platform);
}

void Scene::process(GLFWwindow* window, const LowRenderer::CameraInputs& inputs, const Game::Input& playerInputs, bool gameMode)
{
    update(inputs, playerInputs, gameMode);
    draw(gameMode);
}

void	Scene::update(const LowRenderer::CameraInputs& inputs, const Game::Input& playerInputs, bool gameMode)
{
    updateCamera(inputs, gameMode);
    updateGameObjects(playerInputs);
}

void    Scene::updateGameObjects(const Game::Input& playerInputs)
{

    for (Game::Player& player : players)
    {
        player.update(playerInputs, gameObjects);
    }

    updateColliderPos();
}

void Scene::draw(bool gameMode)
{
    clearBackground();
    drawPlayers();
    drawGameObjects(gameMode);
}

void Resources::Scene::drawPlayers()
{
    for (Game::Player& go : players)
    {
        if (go.model.enabled)
        {
            setModel(go.transform, go.model, go.tag);
            for (int i = 0; i < go.model.meshes.size(); ++i)
            {
                int iplus1 = i + 1;
                if (size_t(iplus1) == go.model.meshes.size())
                {
                    setModel(go.transform, go.model, go.tag, false, true);
                    drawModel(go.model, i, false, true);
                }
                else
                {
                    drawModel(go.model, i);
                }
                    
            }
        }
    }
}

void Resources::Scene::drawGameObjects(bool gameMode)
{
    for (Game::GameObject* go : gameObjects)
    {
        if (go->model.enabled)
        {
            setModel(go->transform, go->model, go->tag);
            for (int i = 0; i < go->model.meshes.size(); ++i)
            {
                int iplus1 = i + 1;
                if (size_t(iplus1) == go->model.meshes.size())
                {
                    setModel(go->transform, go->model, go->tag, false, true);
                    drawModel(go->model, i, false, true);
                }
                else
                {
                    drawModel(go->model, i);
                    if (!gameMode && go->selected)
                    {
                        setModel(go->transform, go->model, go->tag, true);
                        drawModel(go->model, i, true);
                    }
                }
            }
        }
    }
}

void Resources::Scene::updateColliderPos()
{
    for (auto& go : gameObjects)
    {
        switch (go->shape->collider)
        {
        case Core::Maths::Collider::SPHERE:
            go->shape->sph->omega = go->transform.position;
            break;
        case Core::Maths::Collider::BOX:
            go->shape->b->center = go->transform.position;
            break;
        default:
            std::string statement = "invalid collider for gameObject " + static_cast<int>(go->shape->collider);
            Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
            break;
        }
    }
}

void    Scene::clearBackground() const
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void	Scene::drawModel(LowRenderer::Model& model, int index, bool outlinePass, bool isCollider) const
{
    auto& rdrVertex = model.meshes[index].rdrVertices;
    auto& mesh = model.meshes[index];

    if (isCollider)
    {
        if (model.colliderVisible)
        {
            glStencilMask(0x00);
            drawCollider(mesh, rdrVertex);
        }
    }
    else
    {
        if (outlinePass)
        {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            drawGFX(mesh, model, rdrVertex);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
            drawGFX(mesh, model, rdrVertex);
        }
        
    }
    
}

void Resources::Scene::drawCollider(Resources::Mesh& mesh, std::vector<Core::rdrVertex>& rdrVertex) const
{
    glBindTexture(GL_TEXTURE_2D, mesh.texture.texCount);
    glBindVertexArray(mesh.VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    verifyFaceType(mesh, rdrVertex);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Resources::Scene::drawGFX(Resources::Mesh& mesh, LowRenderer::Model& model, std::vector<Core::rdrVertex>& rdrVertex) const
{
    glBindTexture(GL_TEXTURE_2D, mesh.texture.texCount);
    glBindVertexArray(mesh.VAO);

    if (model.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        verifyFaceType(mesh, rdrVertex);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
        verifyFaceType(mesh, rdrVertex);
}

void Resources::Scene::verifyFaceType(Resources::Mesh& mesh, std::vector<Core::rdrVertex>& rdrVertex) const
{
    switch (mesh.faceType)
    {
        case FaceType::TRIANGLE:
            glDrawElements(GL_TRIANGLES, GLsizei(rdrVertex.size()), GL_UNSIGNED_INT, 0);
            break;
        case FaceType::QUAD :
            glDrawElements(GL_TRIANGLES, GLsizei(rdrVertex.size()), GL_UNSIGNED_INT, 0);
            break;
        default:
            std::string statement = "attempt to draw invalid face type " + static_cast<int>(mesh.faceType);
            Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
            break;
    }
}

void Scene::updateCamera(const LowRenderer::CameraInputs& inputs, bool gameMode)
{
    if (gameMode)
        camera.update(inputs, players[0].transform.position);
    else
        camera.update(inputs);
}

void	Scene::setModel(Physics::Transform& transform, LowRenderer::Model& model, Game::Tag& tag, bool outline, bool isCollider)
{
    Core::Maths::mat4 modelMat4;
    if (isCollider)
    {
        modelMat4 = calcModelMat4(transform);
        auto mvp = camera.getMVP(modelMat4);
        model.setColliderShader(mvp);
    }
    else
    {
        auto outlineScale = transform.scale * 0.05f;
        if (outline)
            transform.scale += outlineScale;

        if (tag == Game::Tag::ENEMY || tag == Game::Tag::PLAYER)
        {
            transform.position.y -= transform.scale.y;
            modelMat4 = calcModelMat4(transform);
            transform.position.y += transform.scale.y;
        }
        else
        {
            modelMat4 = calcModelMat4(transform);
        }
        auto mvp = camera.getMVP(modelMat4);
        auto camPos = camera.getCamPos();
        model.setShader(mvp, modelMat4, camPos, outline);
        model.setLights(spotLights, dirLights, pointLights);

        if (outline)
            transform.scale -= outlineScale;
    }
    
}


Core::Maths::mat4	Scene::calcModelMat4(Physics::Transform& transform) const
{
    if (constantTransformations)
    {
        return transform.getModelMatrix(time); 
    }
    return transform.getModelMatrix(); 
}

void Scene::showImGuiControls()
{
    if (ImGui::Begin(name.c_str()))
    {
        if (ImGui::CollapsingHeader("Scene Options", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ColorEdit3("Background", clearColor.e);
        }

        ImGui::Separator();
        ImGui::Columns(2);
        
        if (ImGui::CollapsingHeader("GameObjects"))
        {
            if (ImGui::TreeNode("Cameras"))
            {
                static bool selected = false;
                if (ImGui::Selectable("Camera 00", selected))
                    selected = true;
                if (selected)
                    camera.showImGuiControls();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Players"))
            {
                static int selected = -1;
                for (int i = 0; i < int(players.size()); ++i)
                {
                    std::string label = "Player " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i))
                        selected = i;
                }
                if (selected != -1)
                    players[selected].showImGuiControls();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Enemies"))
            {
                static int selected = -1;
                for (int i = 0; i < int(enemies.size()); ++i)
                {
                    std::string label = "Enemy " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i))
                    {
                        selected = i;
                        enemies[i].selected = true;
                    }
                    else if (selected != -1 && selected != i)
                    {
                        enemies[i].selected = false;
                    }
                }
                if (selected != -1)
                    enemies[selected].showImGuiControls();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Platforms"))
            {
                static int selected = -1;
                for (int i = 0; i < int(platforms.size()); ++i)
                {
                    std::string label = "Platform " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i))
                    {
                        selected = i;
                        platforms[i].selected = true;
                    }
                    else if (selected != -1 && selected != i)
                    {
                        platforms[i].selected = false;
                    }
                        
                }
                if (selected != -1)
                    platforms[selected].showImGuiControls();
                ImGui::TreePop();
            }
        }

        if (ImGui::CollapsingHeader("Lights"))
        {
            if (ImGui::TreeNode("Directional"))
            {
                static int selected = -1;
                for (int i = 0; i < int(dirLights.size()); ++i)
                {
                    std::string label = "Directional Light " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i))
                        selected = i;
                }
                if (selected != -1)
                    dirLights[selected].showImGuiControls();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Point"))
            {
                static int selected = -1;
                for (int i = 0; i < int(pointLights.size()); ++i)
                {
                    std::string label = "Point Light " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i))
                        selected = i;
                }
                if (selected != -1)
                    pointLights[selected].showImGuiControls();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Spot"))
            {
                static int selected = -1;
                for (int i = 0; i < int(spotLights.size()); ++i)
                {
                    std::string label = "Spot Light " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i))
                        selected = i;
                }
                if (selected != -1)
                    spotLights[selected].showImGuiControls();
                ImGui::TreePop();
            }
        }
        
        
    }
    ImGui::End();
}

void Scene::debug()
{
    std::string statement = name + " | Models: " + std::to_string(gameObjects.size()) + " | Point Lights: "
        + std::to_string(pointLights.size()) + " | Directional Lights: " + std::to_string(dirLights.size())
        + " | Spot Lights: " + std::to_string(spotLights.size());
    Core::Debug::Log::print(statement, Core::Debug::LogType::DEBUG);
}
