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
	m_shader = Shader((fs::current_path().string() + "\\resources\\shaders\\phong.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\phong.fs").c_str());
	m_skyShader = Shader((fs::current_path().string() + "\\resources\\shaders\\skybox.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\skybox.fs").c_str());
	m_postProcessingShader = Shader((fs::current_path().string() + "\\resources\\shaders\\postprocessing.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\postprocessing.fs").c_str());

	// generate mesh(es)
	m_model.initialiseSphere(32.0f, 32, 64);

	m_model.material().ambient = Color::Purple().asVec3() * 0.25f;
	m_model.material().diffuse = Color::Purple().asVec3();
	m_model.material().specular = Color::White().asVec3();
	m_model.material().specularPower = 128.0f;

	m_skybox.initialiseBox();
	m_screenQuad.initialiseQuad();

	// load skybox textures
	std::vector<std::string> skyboxTextures;
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sb_strato\\right.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sb_strato\\left.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sb_strato\\up.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sb_strato\\down.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sb_strato\\front.tga");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sb_strato\\back.tga");
	m_cubemap.load(skyboxTextures);

	// set up light
	m_light.ambient = Color::White().asVec3();
	m_light.diffuse = Color::White().asVec3();
	m_light.specular = Color::White().asVec3();

	m_camera.setPosition(glm::vec3(-10, 10, 10));

	/*if (m_renderTarget.initialise(4, m_windowWidth, m_windowHeight) == 0)
	{
		printf("Render Target Error\n");
	}*/
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
	// bind our render target
	//m_renderTarget.bind();

	// clear the color and depth buffers
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable shader
	m_shader.bind();

	float time = (float)glfwGetTime();

	// update light
	m_light.position = glm::vec3(glm::cos(time * 2) * 100,
		100, glm::sin(time * 2) * 100);
	m_shader.setVec3("light.position", m_light.position);
	m_shader.setVec3("light.ambient", m_light.ambient);
	m_shader.setVec3("light.diffuse", m_light.diffuse);
	m_shader.setVec3("light.specular", m_light.specular);

	// send camera position
	m_shader.setVec3("cameraPosition", m_camera.getPosition());

	// enable skybox
	m_shader.setInt("skybox", 18);
	m_cubemap.bind(18);

	// get model matrix
	glm::mat4 model(1);
	//model = glm::scale(model, glm::vec3(1, 1, 1));
	//model = glm::rotate(model, glm::radians(std::sin(time * 30.0f) * 45.0f), glm::vec3(0, 1, 0));
	m_shader.setMat4("ModelMatrix", model);

	// combine matrices
	glm::mat4 pvm = m_camera.getProjectionViewMatrix() * model;
	m_shader.setMat4("ProjectionViewModel", pvm);

	// send normal matrix
	glm::mat3 normalMatrix = glm::inverseTranspose(model);
	m_shader.setMat3("NormalMatrix", normalMatrix);

	m_shader.setFloat("time", time);

	m_model.draw(m_shader);

	// draw skybox last
	glDepthFunc(GL_LEQUAL);
	m_skyShader.bind();
	m_skyShader.setMat4("view", glm::mat4(glm::mat3(m_camera.GetViewMatrix())));
	m_skyShader.setMat4("projection", m_camera.getProjectionMatrix());
	m_skyShader.setInt("skybox", 0);
	m_skybox.draw(m_skyShader);
	glDepthFunc(GL_LESS);

	// unbind target to return to backbuffer
	//m_renderTarget.unbind();

	// render rendertarget onto screen quad
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_postProcessingShader.bind();
	glm::mat4 dummyTransform(1);
	m_postProcessingShader.setMat4("ProjectionViewModel", dummyTransform);
	m_postProcessingShader.setInt("diffuseTexture", 0);
	m_postProcessingShader.setInt("specularTexture", 1);
	m_postProcessingShader.setInt("normalTexture", 2);
	m_postProcessingShader.setInt("alphaTexture", 3);
	m_renderTarget.getTarget(0).bind(0);
	m_renderTarget.getTarget(1).bind(1);
	m_renderTarget.getTarget(2).bind(2);
	m_renderTarget.getTarget(3).bind(3);
	m_postProcessingShader.setFloat("time", time);
	m_screenQuad.draw(m_postProcessingShader);*/

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

	float xoffset = (float)xpos - app->m_lastMousePos.x;
	// y position goes from bottom to top
	float yoffset = app->m_lastMousePos.y - (float)ypos;

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