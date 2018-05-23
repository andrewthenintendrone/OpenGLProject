#include "Camera.h"
#include "Time.h"

// Constructor with vectors
Camera::Camera()
{
	updateTransform();
}

const glm::mat4 Camera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
const glm::mat4 Camera::GetViewMatrix()
{
	//return glm::lookAt(Position, Position + Front, Up);
	return glm::inverse(m_viewMatrix);
}

const glm::mat4 Camera::getProjectionViewMatrix()
{
	return m_projectionMatrix * m_viewMatrix;
}

void Camera::setPosition(const glm::vec3 position)
{
	m_worldPos = glm::vec4(position, 1.0f);
	updateTransform();
}

void Camera::setRotation(const glm::vec3 rotation)
{
	glm::mat4 frontUpRight = glm::mat4(1);
	frontUpRight[0] = m_front;
	frontUpRight[1] = m_up;
	frontUpRight[2] = m_right;

	frontUpRight = glm::rotate(frontUpRight, glm::radians(1.0f), rotation);

	m_front = frontUpRight[0];
	m_up = frontUpRight[1];
	m_right = frontUpRight[2];

	updateTransform();
}

const glm::vec3 Camera::getPosition()
{
	return glm::vec3(m_worldPos);
}

const glm::vec3 Camera::getRotation()
{
	// TODO
	return glm::vec3(1);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
//void Camera::ProcessKeyboard(Camera_Movement direction)
//{
//	float velocity = (run ? MovementSpeed * 10 : MovementSpeed) * Time::getInstance().deltaTime();
//	if (direction == FORWARD)
//		Position += Front * velocity;
//	if (direction == BACKWARD)
//		Position -= Front * velocity;
//	if (direction == LEFT)
//		Position -= Right * velocity;
//	if (direction == RIGHT)
//		Position += Right * velocity;
//}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
//void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
//{
//	xoffset *= MouseSensitivity;
//	yoffset *= MouseSensitivity;
//
//	Yaw += xoffset;
//	Pitch += yoffset;
//
//	// Make sure that when pitch is out of bounds, screen doesn't get flipped
//	if (constrainPitch)
//	{
//		if (Pitch > 89.0f)
//			Pitch = 89.0f;
//		if (Pitch < -89.0f)
//			Pitch = -89.0f;
//	}
//
//	// Update Front, Right and Up Vectors using the updated Euler angles
//	updateCameraVectors();
//}

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
	// Calculate the new Front vector
	//glm::vec3 front;
	//front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//front.y = sin(glm::radians(Pitch));
	//front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	// Up = glm::normalize(glm::cross(Right, Front));

	m_viewMatrix = glm::lookAt(glm::vec3(m_worldPos), glm::vec3(m_worldPos + m_front), glm::vec3(m_worldPos + m_up));
	m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView), (float)m_screenWidth / (float)m_screenHeight, m_nearPlane, m_farPlane);
}
