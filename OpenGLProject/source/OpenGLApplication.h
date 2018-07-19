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
#include "Cubemap.h"
#include "RenderTarget.h"
#include "Color.h"

// OpenGLApplication class that manages everything
class OpenGLApplication
{
public:

	OpenGLApplication(unsigned int width = 1280, unsigned int height = 720, const char* windowTitle = "Open GL");
	~OpenGLApplication();

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
	Shader m_pbrShader;
	Shader* m_shaderToUse = nullptr;

	// Light(s)
	std::vector<DirectionalLight> m_directionalLights;
	std::vector<PointLight> m_pointLights;

	// skybox
	Mesh m_skybox; // skybox mesh
	Shader m_skyboxShader; // skybox shader
	Cubemap m_cubemap; // skybox cubemap texture

	// Mesh(es)
	std::vector<OBJMesh*> m_meshes;

	bool correctGamma = false;
};