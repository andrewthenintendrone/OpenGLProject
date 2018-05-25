#pragma once
#include <GLFW\glfw3.h>
#include <map>

// key state struct
struct KeyState
{
	bool current = false; // is the key currently pressed
	bool previous = false; // was the key pressed last frame
};

// Input class
class Input
{
public:

	static Input& getInstance();

	void setWindowPointer(GLFWwindow* window) { m_window = window; }

	void update();

	bool getPressed(int GLFWkeycode);
	bool getHeld(int GLFWkeycode);
	bool getReleased(int GLFWkeycode);

private:

	// pointer to window
	GLFWwindow* m_window = nullptr;

	// map of keycodes and states
	std::map<int, KeyState> m_keyStates;
};