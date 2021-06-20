#include <stdlib.h>
#include <crtdbg.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.hpp"
#include "core/debug/log.hpp"
#include "time.hpp"

#include "core/debug/memleaks.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtDumpMemoryLeaks();

    Core::Debug::Log::configureLogFiles();

    Application app(framebuffer_size_callback);

    app.run();

    

    return 0;
}