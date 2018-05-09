#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Terrain.h"

// OpenGLApplication class that manages everything
class OpenGLApplication
{
public:

	OpenGLApplication(unsigned int width = 1280, unsigned int height = 720, const char* windowTitle = "Open GL");
	void run();

	// mouse info
	glm::vec2 m_lastMousePos = glm::vec2(-1, -1);

		// Camera
	Camera m_camera = Camera(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0, 1, 0), -45, -15);

private:

	// these should not be called externally
	void setup();
	void update();
	void render();
	void processInput();
	void exit();

	// window width / height
	GLFWwindow* m_window = nullptr;
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	// Shader(s)
	Shader m_shader;

	// Terrain(s)
	Terrain m_terrain;

	// per-frame time logic
	// --------------------
	float m_lastFrame = 0;
	float m_currentFrame = 0;
	float m_deltaTime = 0;
};