#include "OpenGLApplication.h"

#include <glm\gtc\matrix_inverse.hpp>

#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;
#include <iostream>

#include "Time.h"
#include "Color.h"

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

	// move on to setup
	setup();
}

void OpenGLApplication::setup()
{
	// build and compile shader(s)
	m_shader = Shader((fs::current_path().string() + "\\resources\\shaders\\phong.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\phong.fs").c_str());

	// generate mesh(es)
	m_mesh.load(fs::current_path().string() + "\\resources\\objects\\bunny\\bunny.obj", true, true);

	// set up light
	m_light.ambient = glm::vec3(1);
	m_light.diffuse = glm::vec3(1);
	m_light.specular = glm::vec3(1);

	// set up material
	m_material.ambient = Color::MediumPurple().asVec3() * 0.25f;
	m_material.diffuse = Color::MediumPurple().asVec3();
	m_material.specular = Color::White().asVec3();
	m_material.specularPower = 64.0f;
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

	// get projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);

	// get view matrix
	glm::mat4 view = m_camera.GetViewMatrix();

	// get model matrix
	glm::mat4 model(1);
	model = glm::rotate(model, time, glm::vec3(0, 1, 0));
	m_shader.setMat4("ModelMatrix", model);

	// combine matrices
	glm::mat4 pvm = projection * view * model;
	m_shader.setMat4("ProjectionViewModel", pvm);

	// send normal matrix
	glm::mat3 normalMatrix = glm::inverseTranspose(model);
	m_shader.setMat3("NormalMatrix", normalMatrix);

	// update light
	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2),
		//glm::sin(time * 2), 0));
	m_light.direction = glm::vec3(0, 0, -1);
	m_shader.setVec3("light.direction", m_light.direction);
	m_shader.setVec3("light.ambient", m_light.ambient);
	m_shader.setVec3("light.diffuse", m_light.diffuse);
	m_shader.setVec3("light.specular", m_light.specular);

	// update material
	m_shader.setVec3("material.ambient", m_material.ambient);
	m_shader.setVec3("material.diffuse", m_material.diffuse);
	m_shader.setVec3("material.specular", m_material.specular);
	m_shader.setFloat("material.specularPower", m_material.specularPower);

	// send camera position
	m_shader.setVec3("cameraPosition", m_camera.Position);

	// draw mesh
	m_mesh.draw();

	// swap buffers and poll window events
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

// handle any input that has occured
void OpenGLApplication::processInput()
{
	// escape quits the application
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	// left shift enables faster camera movement
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT))
		m_camera.run = true;
	else
		m_camera.run = false;

	// move camera with WASD / arrow keys
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS
		|| glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
		m_camera.ProcessKeyboard(FORWARD);
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS
		|| glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_camera.ProcessKeyboard(BACKWARD);
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS
		|| glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_camera.ProcessKeyboard(LEFT);
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS
		|| glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_camera.ProcessKeyboard(RIGHT);

	// draw in wireframe if space is held
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
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
	app->m_camera.ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse wheel is scrolled this callback is run
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// get OpenGLApplication pointer from inside the window
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	// send scroll to the camera
	app->m_camera.ProcessMouseScroll(yoffset);
}

// whenever the window is resized this callback is run
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// resize viewport to match the new size
	glViewport(0, 0, width, height);
}