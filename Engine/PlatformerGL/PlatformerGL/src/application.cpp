#include <iostream>
#include <fstream>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "application.hpp"
#include "core/debug/log.hpp"
#include "core/debug/assertion.hpp"
#include "time.hpp"

Application::Application(GLFWframebuffersizefun callback)
{
	// ASSERT
	Core::Debug::Assertion::assertTest(SCR_HEIGHT > 0 && SCR_WIDTH > 0);

	std::string statement = "screen width: " + std::to_string(SCR_WIDTH) + " | screen height: " + 
		std::to_string(SCR_HEIGHT);
	Core::Debug::Log::print(statement, Core::Debug::LogType::DEBUG);

    init(callback);
}

void Application::init(GLFWframebuffersizefun callback)
{
    initglfw();
    createWindow(callback);
    loadGlad();
    loadImGui();
}

void Application::initglfw()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

void Application::loadGlad()
{
    // glad: load all OpenGL function pointers
    // --------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		std::string statement = "Failed to initialize GLAD";
        Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
    }
    else
    {
		std::string statement = "GLAD successfully initialized";
        Core::Debug::Log::print(statement, Core::Debug::LogType::INFO);
    }
}

void Application::createWindow(GLFWframebuffersizefun framebuffer_size_callback)
{
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloGlWindow", NULL, NULL);
    if (window == NULL)
    {
		std::string statement = "Failed to create GLFW window";
        Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Application::loadImGui()
{
    // Setup Dear ImGui
	
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void Application::run()
{
	gameLoop();

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::newFrame()
{
	glfwPollEvents();
	
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    if (mouseCaptured)
        ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    ImGui::NewFrame();
}

void Application::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Application::gameLoop()
{
	glEnable(GL_DEPTH_TEST);
	

	bool lastLogStatus = logsEnabled;

	playerInputs.jumpState = GLFW_RELEASE;

	while (!glfwWindowShouldClose(window))
	{
		Time::getInstance().update();

		processInput();

		switch (gs)
		{
		case GameState::INMENU:
			menu();
			break;

		case GameState::INGAME:
			render();
			break;

		case GameState::LOSE:
			break;

		case GameState::WIN:
			break;

		default:
			break;
		}
	}
}

void Application::render()
{
	newFrame();

	updateMousePos();

	verifyMouseCapture();
	updateInputs();

	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
	Resources::Scene& scene = graph.getScene(currScene);
	scene.process(window, inputs, playerInputs, gameMode);

	showImGuiControls();
	scene.showImGuiControls();
        
	verifyMouseCapture();

	endFrame();
}

void Application::menu()
{
	newFrame();

	updateMousePos();

	mouseCaptured = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	updateInputs();

	Core::Maths::vec3 clearColor = { 0.f, 0.f, 0.f };
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ImGui::ShowDemoWindow();

	if (ImGui::Begin("PlatformerGL"))
	{
		if (ImGui::Button("New Game"))
		{
			//load scenes
			if (!graph.scenesLoaded)
			{
				graph.loadScenes();
				createVAO();
				graph.setScenes();
			}

			gs = GameState::INGAME;
			Time::timeScale() = 1.f;
		}
		if (graph.hasSavedScene())
		{
			if (ImGui::Button("Load game"))
			{
				if (!graph.scenesLoaded)
				{
					graph.loadSavedScene();
					createVAO();
					graph.setScenes();
				}

				gs = GameState::INGAME;
				Time::timeScale() = 1.f;
			}
		}
		//if (ImGui::Button("Options"))

		if (ImGui::Button("Quit"))
			glfwSetWindowShouldClose(window, true);
	}
	ImGui::End();

	endFrame();
}

void Application::showImGuiControls()
{
	
	if (ImGui::Begin("Application"))
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Scenes"))
		{
			ImGui::SliderInt("Index", &currScene, 0, int(graph.rm.scenes.size()) - 1);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Info"))
		{
			if (ImGui::CollapsingHeader("Mouse", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("MousePos: %f, %f", mouseX, mouseY);
			}
			ImGui::TreePop();
		}
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Settings"))
		{
			if (ImGui::CollapsingHeader("Mode", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("Game Mode", &gameMode);
			}
			if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("Logs Enabled", &Core::Debug::Log::enabled);
				ImGui::Checkbox("Asserts Enabled", &Core::Debug::Assertion::enabled);
			}
			ImGui::TreePop();
		}
		
	}
	ImGui::End();
	
}

void Application::updateMousePos()
{
	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);
	mouseDeltaX = (float)(newMouseX - mouseX);
	mouseDeltaY = (float)(newMouseY - mouseY);
	mouseX = newMouseX;
	mouseY = newMouseY;
}

void Application::verifyMouseCapture()
{
	if (glfwGetKey(window, GLFW_KEY_0))
	{
		mouseCaptured = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (glfwGetKey(window, GLFW_KEY_9))
	{
		mouseCaptured = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Application::updateInputs()
{
	if (mouseCaptured)
	{
		if (gameMode)
		{
			updatePlayerInputs();
		}
		else
		{
			updateCameraInputs();
		}
	}
	
}

void Application::updatePlayerInputs()
{
	playerInputs.jump = getJumpInput();
	playerInputs.moveForward = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	playerInputs.moveBackward = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	playerInputs.moveLeft = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	playerInputs.moveRight = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

bool Application::getJumpInput()
{
	auto newJumpState = glfwGetKey(window, GLFW_KEY_SPACE);
	if (newJumpState == GLFW_PRESS && playerInputs.jumpState == GLFW_RELEASE)
	{
		playerInputs.jumpState = newJumpState;
		return true;
	}

	playerInputs.jumpState = newJumpState;
	return false;
}

void Application::updateCameraInputs()
{
	inputs.deltaX = mouseDeltaX;
	inputs.deltaY = mouseDeltaY;
	inputs.moveForward = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	inputs.moveBackward = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	inputs.strafeLeft = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	inputs.strafeRight = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	inputs.moveUp = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	inputs.moveDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput()
{
	switch (gs)
	{
	case GameState::INMENU:
		break;

	case GameState::INGAME:
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			graph.unloadScenes();

			gs = GameState::INMENU;
			Time::timeScale() = 0.f;
		}
		if (glfwGetKey(window, GLFW_KEY_F5))
		{
			graph.saveScenes();
		}

		break;

	case GameState::LOSE:
		break;

	case GameState::WIN:
		break;

	default:
		break;
	}
}

void Application::createVAO()
{
	for (Resources::Scene& scene : graph.rm.scenes)
	{
		for (Game::Player& go : scene.players)
		{
			go.defineVAO();
		}
		for (Game::Enemy& go : scene.enemies)
		{
			go.defineVAO();
		}
		for (Game::Platform& go : scene.platforms)
		{
			go.defineVAO();
		}
	}

}