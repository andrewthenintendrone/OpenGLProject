#include "Input.h"

Input& Input::getInstance()
{
	static Input instance;
	return instance;
}

// update all key states in the map
void Input::update()
{
	for (auto iter = m_keyStates.begin(); iter != m_keyStates.end(); iter++)
	{
		iter->second.previous = iter->second.current;
		iter->second.current = glfwGetKey(m_window, iter->first);
	}
}

bool Input::getPressed(int GLFWkeycode)
{
	// search for the key in the map
	std::map<int, KeyState>::iterator iter = m_keyStates.find(GLFWkeycode);

	// if it does not exist create it
	if (iter == m_keyStates.end())
	{
		m_keyStates.insert(std::pair<int, KeyState>(GLFWkeycode, KeyState()));
		return false;
	}
	// otherwise return if the key was pressed this frame
	else
	{
		KeyState state = iter->second;

		return (state.current && !state.previous);
	}
}

bool Input::getHeld(int GLFWkeycode)
{
	// search for the key in the map
	auto iter = m_keyStates.find(GLFWkeycode);

	// if it does not exist create it
	if (iter == m_keyStates.end())
	{
		m_keyStates.insert(std::pair<int, KeyState>(GLFWkeycode, KeyState()));
		return false;
	}
	// otherwise return if the key is currently pressed
	else
	{
		KeyState state = iter->second;

		return (state.current);
	}
}

bool Input::getReleased(int GLFWkeycode)
{
	// search for the key in the map
	auto iter = m_keyStates.find(GLFWkeycode);

	// if it does not exist create it
	if (iter == m_keyStates.end())
	{
		m_keyStates.insert(std::pair<int, KeyState>(GLFWkeycode, KeyState()));
		return false;
	}
	// otherwise return if the key was released this frame
	else
	{
		KeyState state = iter->second;

		return (!state.current && state.previous);
	}
}
