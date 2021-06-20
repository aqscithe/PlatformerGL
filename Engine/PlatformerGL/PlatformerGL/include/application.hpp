#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/datastructure/graph.hpp"

enum class GameState
{
	INMENU,
	INGAME,
	LOSE,
	WIN,
};

class Application 
{
	public:
		Application(GLFWframebuffersizefun callback);
		
		Application(const Application& app) = delete;
		void	operator=(const Application& app) = delete;
		
		void						run();

	private:
		void						init(GLFWframebuffersizefun callback);
		void						initglfw();
		void						createWindow(GLFWframebuffersizefun callback);

		void						loadGlad();
		void						loadImGui();

		void						processInput();

		void						newFrame();
		void						endFrame();

		void						updateMousePos();
		void						verifyMouseCapture();

		void						updateInputs();
		void						updatePlayerInputs();
		bool						getJumpInput();
		void						updateCameraInputs();

		//game
		void						gameLoop();
		void						render();
		void						menu();

		void						showImGuiControls();

		void						createVAO();

		GLFWwindow*					window;

		LowRenderer::CameraInputs	inputs;
		Game::Input					playerInputs;

		Core::DataStructure::Graph	graph;

		const unsigned int			SCR_WIDTH = 1920;
		const unsigned int			SCR_HEIGHT = 1080;

		bool						logsEnabled = true;
		bool						mouseCaptured = false;
		bool						gameMode = true;

		double						mouseX = 0.0;
		double						mouseY = 0.0;

		float						mouseDeltaX = 0.0;
		float						mouseDeltaY = 0.0;

		int							currScene = 0;

		GameState					gs = GameState::INMENU;
};