#include "OpenGLApplication.h"

#include <glm\gtc\matrix_inverse.hpp>

#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;
#include <iostream>

#include "Time.h"
#include "Color.h"
#include "Input.h"

// callback functions
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

OpenGLApplication::OpenGLApplication(unsigned int width, unsigned int height, const char* windowTitle) : m_windowWidth(width), m_windowHeight(height)
{
	// initialise glfw
	glfwInit();

	// tell glfw we will be using OpenGL 4.3 core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	m_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// check that the window was created sucessfully
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	// store a pointer to this instance of OpenGLApplication within the window
	glfwSetWindowUserPointer(m_window, this);

	// set up the various callback functions
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	// tell GLFW to lock and hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialise glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// enable z buffer
	glEnable(GL_DEPTH_TEST);
	// enable front face culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	// set Input window pointer
	Input::getInstance().setWindowPointer(m_window);

	// move on to setup
	setup();
}

void OpenGLApplication::setup()
{
	// build and compile shader(s)
	m_shader = Shader((fs::current_path().string() + "\\resources\\shaders\\mirror.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\mirror.fs").c_str());
	m_skyShader = Shader((fs::current_path().string() + "\\resources\\shaders\\skybox.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\skybox.fs").c_str());

	// generate mesh(es)
	m_mesh.load(fs::current_path().string() + "\\resources\\objects\\Mario\\Mario.obj", true, true);
	m_skybox.initialiseBox();

	// load skybox textures
	std::vector<std::string> skyboxTextures;
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\mp_sorbin\\right.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\mp_sorbin\\left.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\mp_sorbin\\up.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\mp_sorbin\\down.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\mp_sorbin\\front.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\mp_sorbin\\back.tga");
	m_cubemap.load(skyboxTextures);

	// set up light
	m_light.ambient = Color::White().asVec3();
	m_light.diffuse = Color::White().asVec3();
	m_light.specular = Color::White().asVec3();

	// set up material
	m_material.ambient = Color::White().asVec3() * 0.25f;
	m_material.diffuse = Color::White().asVec3();
	m_material.specular = Color::White().asVec3();

	m_camera.setPosition(glm::vec3(-10, 10, 10));
}

void OpenGLApplication::run()
{
	// continue to update and render while the window is still open
	while (!glfwWindowShouldClose(m_window))
	{
		update();
		render();
	}

	// clean up and exit
	exit();
}

void OpenGLApplication::update()
{
	// update Time
	Time::getInstance().update();

	// process input
	processInput();
}

void OpenGLApplication::render()
{
	// clear the color and depth buffers
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable shader
	m_shader.use();

	float time = glfwGetTime();

	// update light
	m_light.position = glm::vec3(glm::cos(time * 2) * 100,
		100, glm::sin(time * 2) * 100);
	m_shader.setVec3("light.position", m_light.position);
	m_shader.setVec3("light.ambient", m_light.ambient);
	m_shader.setVec3("light.diffuse", m_light.diffuse);
	m_shader.setVec3("light.specular", m_light.specular);

	// update material
	m_shader.setVec3("material.ambient", m_material.ambient);
	m_shader.setVec3("material.diffuse", m_material.diffuse);
	m_shader.setVec3("material.specular", m_material.specular);
	m_shader.setFloat("material.emissive", std::sin(time * 3.0f) * 0.5f + 0.5f);
	m_shader.setFloat("material.roughness", 0.25f);
	m_shader.setFloat("material.reflectionCoefficient", 0.25f);

	// send camera position
	m_shader.setVec3("cameraPosition", m_camera.getPosition());

	m_shader.setInt("skybox", 0);

	// get model matrix
	glm::mat4 model(1);
	m_shader.setMat4("ModelMatrix", model);

	// combine matrices
	glm::mat4 pvm = m_camera.getProjectionViewMatrix() * model;
	m_shader.setMat4("ProjectionViewModel", pvm);

	// send normal matrix
	glm::mat3 normalMatrix = glm::inverseTranspose(model);
	m_shader.setMat3("NormalMatrix", normalMatrix);

	m_mesh.draw();

	// draw skybox last
	glDepthFunc(GL_LEQUAL);
	m_skyShader.use();
	m_skyShader.setMat4("view", glm::mat4(glm::mat3(m_camera.GetViewMatrix())));
	m_skyShader.setMat4("projection", m_camera.getProjectionMatrix());
	m_skyShader.setInt("skybox", 0);
	m_skybox.draw(m_skyShader);
	glDepthFunc(GL_LESS);

	// swap buffers and poll window events
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

// handle any input that has occured
void OpenGLApplication::processInput()
{
	// update Input
	Input::getInstance().update();

	// escape exits
	if (Input::getInstance().getPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window, true);

	// left shift enables faster camera movement
	if (Input::getInstance().getHeld(GLFW_KEY_LEFT_SHIFT))
	 m_camera.m_running = true;
	 else
	 m_camera.m_running = false;

	// move camera with WASD / arrow keys
	if (Input::getInstance().getHeld(GLFW_KEY_W) || Input::getInstance().getHeld(GLFW_KEY_UP))
		m_camera.processKeyboard(FORWARD);
	if (Input::getInstance().getHeld(GLFW_KEY_S) || Input::getInstance().getHeld(GLFW_KEY_DOWN))
		m_camera.processKeyboard(BACKWARD);
	if (Input::getInstance().getHeld(GLFW_KEY_A) || Input::getInstance().getHeld(GLFW_KEY_LEFT))
		m_camera.processKeyboard(LEFT);
	if (Input::getInstance().getHeld(GLFW_KEY_D) || Input::getInstance().getHeld(GLFW_KEY_RIGHT))
		m_camera.processKeyboard(RIGHT);

	// draw in wireframe if space is held
	if (Input::getInstance().getHeld(GLFW_KEY_SPACE))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLApplication::exit()
{
	// terminate glfw
	glfwTerminate();
}

// whenever the mouse is moved this callback is run
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// get OpenGLApplication pointer from inside the window
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	// this is the first mouse movement
	if (app->m_lastMousePos == glm::vec2(-1, -1))
	{
		app->m_lastMousePos = glm::vec2(xpos, ypos);
	}

	float xoffset = xpos - app->m_lastMousePos.x;
	// y position goes from bottom to top
	float yoffset = app->m_lastMousePos.y - ypos;

	// store the last mouse position
	app->m_lastMousePos = glm::vec2(xpos, ypos);

	// send mouse movement to the camera
	app->m_camera.processMouseMovement(xoffset, yoffset);
}

// whenever the mouse wheel is scrolled this callback is run
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// get OpenGLApplication pointer from inside the window
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	// send scroll to the camera
	// app->m_camera.ProcessMouseScroll(yoffset);
}

// whenever the window is resized this callback is run
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// resize viewport to match the new size
	glViewport(0, 0, width, height);
}