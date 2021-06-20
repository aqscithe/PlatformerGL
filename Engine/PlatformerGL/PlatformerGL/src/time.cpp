#include "time.hpp"

#include <iostream>

#include <GLFW/glfw3.h>

float& Time::timeScale()
{
	return getInstance().ts;
}

float Time::deltaTime()
{
	return getInstance().dt * getInstance().ts;
}

float Time::fixedDeltaTime()
{
	return getInstance().fdt * getInstance().ts;
}

int Time::fixing()
{
	return getInstance().f == 0 ? 1 : getInstance().f;
}

void Time::resetLastTime()
{
	getInstance().lastTime = glfwGetTime();
}

void Time::update()
{
	float curTime = glfwGetTime();

	// delta time
	// ----------
	dt = curTime - lastTime;
	lastTime = curTime;

	// fixed delta time
	// ----------------
	f = 0;
	float temp = dt;
	while (temp > fdt)
	{
		temp -= fdt;

		++f;
	}
}