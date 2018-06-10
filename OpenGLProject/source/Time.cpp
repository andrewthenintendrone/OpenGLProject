#include "Time.h"
#include <GLFW\glfw3.h>
#include <iostream>

Time& Time::getInstance()
{
	static Time instance;
	return instance;
}

// update Time
void Time::update()
{
	// update deltaTime
	m_currentFrame = (float)glfwGetTime();
	m_deltaTime = m_currentFrame - m_lastFrame;
	m_lastFrame = m_currentFrame;

	m_fps = 1.0f / m_deltaTime;
}
