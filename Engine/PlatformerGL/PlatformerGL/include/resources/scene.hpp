#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "lowrenderer/camera.hpp"
#include "lowrenderer/directionallight.hpp"
#include "lowrenderer/spotlight.hpp"
#include "lowrenderer/pointlight.hpp"
#include "lowrenderer/model.hpp"
#include "game/gameobject.hpp"
#include "game/player.hpp"
#include "game/enemy.hpp"
#include "game/platform.hpp"
#include "core/maths/sphere.hpp"
#include "core/maths/box.hpp"

namespace Resources
{
	class Scene
	{
	public:
		Scene() = default;
		Scene(const std::string& scnFile);
		~Scene();

		void								setGameObjects();
		void								process(
												GLFWwindow* window,
												const LowRenderer::CameraInputs& inputs
												, const Game::Input& playerInputs, bool gameMode
											);
		void								showImGuiControls();

		void								addGameObject(
												const std::vector<float>& modelAttribs, 
												const std::string& colliderName,
												const std::vector<float>& colliderAttribs, 
												const std::vector<int>& gameObjAttrib,
												const std::string& customTexture
											);
		
		void								debug();

		std::vector<Game::Player>					players;
		std::vector<Game::Enemy>					enemies;
		std::vector<Game::Platform>					platforms;
		std::vector<Game::GameObject*>				gameObjects;

		std::vector<LowRenderer::PointLight>		pointLights;
		std::vector<LowRenderer::DirectionalLight>	dirLights;
		std::vector<LowRenderer::SpotLight>			spotLights;

		LowRenderer::Camera							camera;

		std::string name;

	private:
		void								updateColliderPos();
		void								clearBackground() const;
		void								drawModel(LowRenderer::Model& model, int index, bool outlinePass = false, bool isCollider = false) const;
		void drawCollider(Resources::Mesh& mesh, std::vector<Core::rdrVertex>& rdrVertex) const;
		void drawGFX(Resources::Mesh& mesh, LowRenderer::Model& model, std::vector<Core::rdrVertex>& rdrVertex) const;
		void verifyFaceType(Resources::Mesh& mesh, std::vector<Core::rdrVertex>& rdrVertex) const;
		void								setModel(Physics::Transform& transform, LowRenderer::Model& model, Game::Tag& tag, bool outlinePass = false, bool isCollider = false);
		void								update(const LowRenderer::CameraInputs& inputs, const Game::Input& playerInputs, bool gameMode);
		void								updateCamera(const LowRenderer::CameraInputs& inputs, bool gameMode);
		void								updateGameObjects(const Game::Input& playerInputs);
		void								draw(bool gameMode);
		void								drawPlayers();
		void								drawGameObjects(bool gameMode);

		Core::Maths::mat4					calcModelMat4(Physics::Transform& transform) const;

		Core::Maths::vec3					clearColor{ 0.3f, 0.8f, 0.5f };

		int									currGameObj = 0;
		int									currPlayer = 0;
		int									currEnemy = 0;
		int									currDir = 0;
		int									currPoint = 0;
		int									currSpot = 0;
		float								time = 0.f;
		float								modelColliderOffset = 1.f;

		bool								constantTransformations = false;
	};
}