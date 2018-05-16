#include "OpenGLApplication.h"

#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

#include "Color.h"

// callback functions
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

OpenGLApplication::OpenGLApplication(unsigned int width, unsigned int height, const char* windowTitle) : m_windowWidth(width), m_windowHeight(height)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	m_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	glfwSetWindowUserPointer(m_window, this);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);

	setup();
}

void OpenGLApplication::setup()
{
	// build and compile shaders
	m_shader = Shader((fs::current_path().string() + "\\resources\\shaders\\simple.vs").c_str(), (fs::current_path().string() + "\\resources\\shaders\\simple.fs").c_str());

	// load models
	// m_model = Model(fs::current_path().string() + "\\resources\\objects\\brush\\brush.obj");
	// m_terrain = Terrain(256, 256);
	// m_terrain.generatePerlin();
	mesh.initialiseCylinder(3, 6, 8);
}

void OpenGLApplication::run()
{
	// render loop
	// -----------
	while (!glfwWindowShouldClose(m_window))
	{
		update();
		render();
	}

	exit();
}

void OpenGLApplication::update()
{
	// per-frame time logic
	// --------------------
	m_currentFrame = glfwGetTime();
	m_deltaTime = m_currentFrame - m_lastFrame;
	m_lastFrame = m_currentFrame;

	// input
	// -----
	processInput();
}

void OpenGLApplication::render()
{
	// render
	// ------
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	m_shader.use();

	float time = glfwGetTime() * 3.0f;

	// projection / view / model transformations
	glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);
	glm::mat4 view = m_camera.GetViewMatrix();
	glm::mat4 model(1);
	model = glm::scale(model, glm::vec3(10));
	
	// combine matrices
	glm::mat4 pvmMatrix = projection * view * model;

	m_shader.setMat4("ProjectionViewModel", pvmMatrix);
	m_shader.setVec4("drawColor", Color::Splatoon2Blue().asVec4());

	mesh.draw(m_shader);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void OpenGLApplication::processInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT))
		m_camera.run = true;
	else
		m_camera.run = false;

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera.ProcessKeyboard(FORWARD, m_deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera.ProcessKeyboard(BACKWARD, m_deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera.ProcessKeyboard(LEFT, m_deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera.ProcessKeyboard(RIGHT, m_deltaTime);

	// draw in wireframe if space is held
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// adjust shininess with up and down
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_shininess = std::min(256.0f, m_shininess + m_deltaTime * 100.0f);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_shininess = std::max(32.0f, m_shininess - m_deltaTime * 100.0f);
	}
}

void OpenGLApplication::exit()
{
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	if (app->m_lastMousePos == glm::vec2(-1, -1))
	{
		app->m_lastMousePos = glm::vec2(xpos, ypos);
	}

	float xoffset = xpos - app->m_lastMousePos.x;
	float yoffset = app->m_lastMousePos.y - ypos; // reversed since y-coordinates go from bottom to top

	app->m_lastMousePos = glm::vec2(xpos, ypos);

	app->m_camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	app->m_camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}