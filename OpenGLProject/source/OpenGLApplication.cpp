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

	// tell glfw we will be using OpenGL version 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window using glfw
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

	// enable the z buffer
	glEnable(GL_DEPTH_TEST);

	// enable front face culling
	glEnable(GL_CULL_FACE);

	// set Input window pointer
	Input::getInstance().setWindowPointer(m_window);

	// move on to setup
	setup();
}

// load and create all the various assets needed
void OpenGLApplication::setup()
{
	// load and compile shaders
	m_phongShader = Shader((fs::current_path().string() + "\\resources\\shaders\\phong.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\phong.fs").c_str());
	m_skyboxShader = Shader((fs::current_path().string() + "\\resources\\shaders\\skybox.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\skybox.fs").c_str());

	// generate procedural mesh
	m_proceduralMesh.initialiseIcosahedron();
	m_proceduralMesh.material().ambient = Color::White().asVec3();
	m_proceduralMesh.material().diffuse = Color::White().asVec3();
	m_proceduralMesh.material().specular = Color::White().asVec3();
	m_proceduralMesh.material().roughness = 0.5f;
	m_proceduralMesh.material().reflectionCoefficient = 0.5f;

	// apply textures to the procedural mesh
	m_proceduralMesh.material().diffuseTexture.load((fs::current_path().string() + "\\resources\\textures\\earth\\earth.png").c_str());
	m_proceduralMesh.material().ambientTexture.load((fs::current_path().string() + "\\resources\\textures\\earth\\earth_night.png").c_str());
	m_proceduralMesh.material().specularTexture.load((fs::current_path().string() + "\\resources\\textures\\earth\\earth_spec.png").c_str());
	m_proceduralMesh.material().normalTexture.load((fs::current_path().string() + "\\resources\\textures\\earth\\earth_normal.png").c_str());

	// load character mesh
	m_characterMesh.load((fs::current_path().string() + "\\resources\\objects\\Waluigi\\Waluigi.obj").c_str());

	// procedually create skybox mesh
	m_skybox.initialiseBox();

	// load skybox textures into a cubemap
	std::vector<std::string> skyboxTextures;
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sky2\\right.png");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sky2\\left.png");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sky2\\up.png");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sky2\\down.png");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sky2\\front.png");
	skyboxTextures.push_back(fs::current_path().string() + "\\resources\\textures\\sky2\\back.png");
	m_cubemap.load(skyboxTextures);

	// set up lights
	m_pointLight.ambient = Color::White().asVec3();
	m_pointLight.diffuse = Color::White().asVec3();
	m_pointLight.specular = Color::White().asVec3();

	m_directionalLight.direction = glm::normalize(glm::vec3(1, -1, -1));

	// set camera position
	m_camera.setPosition(glm::vec3(0, 15, 25));
	m_camera.setLookAt(glm::vec3(0, 15, 0));

	m_terrain = Terrain(128, 128);
	m_terrain.generatePerlin();
	m_terrain.material().ambientTexture.load("C:\\Users\\s170837\\Pictures\\terrain\\color\\0_Plant_GreenGrassField_A.png");
	m_terrain.material().diffuseTexture.load("C:\\Users\\s170837\\Pictures\\terrain\\color\\0_Plant_GreenGrassField_A.png");
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

	// bind phong shader
	m_phongShader.bind();

	// store time as a float
	float time = (float)glfwGetTime();

	// update directional light
	m_directionalLight.bind(m_phongShader);

	// update point light
	m_pointLight.position = glm::vec3(std::sin(time) * 200.0f, 45.0f, std::cos(time) * 200.0f);
	m_pointLight.bind(m_phongShader);

	// send camera position
	m_phongShader.setVec3("cameraPosition", m_camera.getPosition());

	// enable skybox
	m_phongShader.setInt("skybox", 18);
	m_cubemap.bind(18);

	// create model matrix
	glm::mat4 model(1);
	model = glm::scale(model, glm::vec3(25.0f));
	m_phongShader.setMat4("ModelMatrix", model);

	// create projection view model matrix
	glm::mat4 pvm = m_camera.getProjectionViewMatrix() * model;
	m_phongShader.setMat4("ProjectionViewModel", pvm);

	// create normal matrix
	glm::mat3 normalMatrix = glm::inverseTranspose(model);
	m_phongShader.setMat3("NormalMatrix", normalMatrix);

	// draw procedural mesh
	m_phongShader.setFloat("TessLevelInner", 1);
	m_phongShader.setFloat("TessLevelOuter", 1);
	m_proceduralMesh.draw(m_phongShader);

	// terrain
	model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(1.0f, 64.0f, 1.0f));
	m_phongShader.setMat4("ModelMatrix", model);

	// create projection view model matrix
	pvm = m_camera.getProjectionViewMatrix() * model;
	m_phongShader.setMat4("ProjectionViewModel", pvm);

	// create normal matrix
	normalMatrix = glm::inverseTranspose(model);
	m_phongShader.setMat3("NormalMatrix", normalMatrix);
	m_terrain.draw(m_phongShader);

	// recreate model matrix for character mesh
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(0, 32, 0));
	model = glm::scale(model, glm::vec3(2.0f));
	m_phongShader.setMat4("ModelMatrix", model);

	// recreate projection view model matrix
	pvm = m_camera.getProjectionViewMatrix() * model;
	m_phongShader.setMat4("ProjectionViewModel", pvm);

	// recreate normal matrix
	normalMatrix = glm::inverseTranspose(model);
	m_phongShader.setMat3("NormalMatrix", normalMatrix);

	m_phongShader.setVec3("cameraPosition", m_camera.getPosition());
	m_phongShader.setInt("skybox", 18);

	// draw character mesh
	m_characterMesh.draw(m_phongShader);

	// draw skybox

	// use less than or equal for the depth function to allow the skybox to draw when the z buffer is empty
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	// bind skybox shader
	m_skyboxShader.bind();

	// remove the translation component of the view matrix for the skybox
	m_skyboxShader.setMat4("view", glm::mat4(glm::mat3(m_camera.GetViewMatrix())));
	m_skyboxShader.setMat4("projection", m_camera.getProjectionMatrix());

	// bind the cubemap to slot 0
	m_cubemap.bind(0);
	m_skyboxShader.setInt("skybox", 0);

	// draw skybox
	m_skybox.draw(m_skyboxShader);

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

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