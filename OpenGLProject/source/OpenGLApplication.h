#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "FlyCamera.h"
#include "Light.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Terrain.h"
#include "Cubemap.h"
#include "RenderTarget.h"
#include "Color.h"

// OpenGLApplication class that manages everything
class OpenGLApplication
{
public:

	OpenGLApplication(unsigned int width = 1280, unsigned int height = 720, const char* windowTitle = "Open GL");
	void run();

	// mouse info
	glm::vec2 m_lastMousePos = glm::vec2(-1, -1);

	// Camera
	FlyCamera m_camera;

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
	Shader m_phongShader;

	// Light(s)
	DirectionalLight m_directionalLight;
	PointLight m_pointLight;

	// skybox
	Mesh m_skybox; // skybox mesh
	Shader m_skyboxShader; // skybox shader
	Cubemap m_cubemap; // skybox cubemap texture

	// Mesh(es)
	Mesh m_proceduralMesh; // procedurally generated mesh
	OBJMesh m_characterMesh; // character mesh
	Terrain m_terrain;
};