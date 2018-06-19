#include "Camera.h"
#include "Time.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	updateProjectionMatrix();
	updateViewMatrix();
	updateProjectionViewMatrix();
}

// set the position of the camera
void Camera::setPosition(const glm::vec3 position)
{
	m_position = position;
	updateViewMatrix();
	updateProjectionViewMatrix();
}

// set the look at position of the camera
void Camera::setLookAt(const glm::vec3 lookAt)
{
	m_lookAt = lookAt;
	updateViewMatrix();
	updateProjectionViewMatrix();
}

// update the projection matrix (done only when the screen size or FOV changes)
void Camera::updateProjectionMatrix()
{
	m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView), (float)m_screenWidth / (float)m_screenHeight, m_nearPlane, m_farPlane);
}

// update the view matrix (done when needed)
void Camera::updateViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_position, m_lookAt, glm::vec3(0, 1, 0));
}

// update the projection view matrix (done whenever view or projection matrix is changed)
void Camera::updateProjectionViewMatrix()
{
	m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
}
