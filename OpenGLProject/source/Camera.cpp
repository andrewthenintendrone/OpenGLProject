#include "Camera.h"
#include "Time.h"

const glm::mat4 Camera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

// returns the view matrix
const glm::mat4 Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

// returns the projection matrix
const glm::mat4 Camera::getProjectionViewMatrix()
{
	return m_projectionMatrix * m_viewMatrix;
}

// set the position of the camera
void Camera::setPosition(const glm::vec3 position)
{
	m_position = glm::vec4(position, 1.0f);
	updateTransform();
}

void Camera::setLookAt(const glm::vec3 lookAt)
{
	m_targetPosition = lookAt;
	updateTransform();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
//void Camera::ProcessMouseScroll(float yoffset)
//{
//	if (Zoom >= 1.0f && Zoom <= 45.0f)
//		Zoom -= yoffset;
//	if (Zoom <= 1.0f)
//		Zoom = 1.0f;
//	if (Zoom >= 45.0f)
//		Zoom = 45.0f;
//}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateTransform()
{
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_targetPosition), glm::vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView), (float)m_screenWidth / (float)m_screenHeight, m_nearPlane, m_farPlane);
}
