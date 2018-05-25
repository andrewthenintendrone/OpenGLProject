#pragma once
#include "Camera.h"
#include <GLFW\glfw3.h>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class FlyCamera : public Camera
{
public:

	bool m_running = false;

	FlyCamera();

	void setPosition(const glm::vec3 position);

	void processKeyboard(Camera_Movement direction);
	void processMouseMovement(float xoffset, float yoffset);

private:

	void updateTransform();

	glm::vec3 m_front = glm::vec3(0, 0, 1);
	glm::vec3 m_right = glm::vec3(1, 0, 0);
	glm::vec3 m_up = glm::vec3(0, 1, 0);

	float m_mouseSensitivity = 0.1f;

	float m_pitch = -45.0f;
	float m_yaw = -45.0f;

	float m_walkSpeed = 10.0f;
	float m_runSpeed = 50.0f;
};