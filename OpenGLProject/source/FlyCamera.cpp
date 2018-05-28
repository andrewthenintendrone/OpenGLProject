#include "FlyCamera.h"
#include "Time.h"
#include <algorithm>

FlyCamera::FlyCamera()
{
	
}

void FlyCamera::setPosition(const glm::vec3 position)
{
	m_position = position;

	updateTransform();
}

void FlyCamera::processKeyboard(Camera_Movement direction)
{
	float velocity = (m_running ? m_runSpeed : m_walkSpeed) * Time::getInstance().deltaTime();

	switch (direction)
	{
	case FORWARD:
		m_position += m_front * velocity;
		break;
	case BACKWARD:
		m_position -= m_front * velocity;
		break;
	case LEFT:
		m_position -= m_right * velocity;
		break;
	case RIGHT:
		m_position += m_right * velocity;
		break;
	default:
		break;
	}

	updateTransform();
}

void FlyCamera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	m_pitch = std::min(89.0f, m_pitch);
	m_pitch = std::max(-89.0f, m_pitch);

	updateTransform();
}

void FlyCamera::updateTransform()
{
	// Calculate front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0, 1, 0)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));

	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
	m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView), (float)m_screenWidth / (float)m_screenHeight, m_nearPlane, m_farPlane);
}
