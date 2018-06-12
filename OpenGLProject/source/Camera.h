#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

// An abstract camera class
class Camera
{
public:

	// constructors
	Camera();

	const glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }
	const glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	const glm::mat4 getProjectionViewMatrix() { return m_projectionViewMatrix; }

	void setPosition(const glm::vec3 position);
	void setLookAt(const glm::vec3 lookAt);

	const glm::vec3 getPosition() { return m_position; }

protected:

	void updateProjectionMatrix();
	void updateViewMatrix();
	void updateProjectionViewMatrix();

	// Camera Attributes
	glm::vec3 m_position = glm::vec4(0, 0, 0, 1); // position in world space
	glm::vec3 m_targetPosition = glm::vec3(0); // position to look at in world space

	// current view and projection matrices
	glm::mat4 m_viewMatrix = glm::mat4(1);
	glm::mat4 m_projectionMatrix = glm::mat4(1);
	glm::mat4 m_projectionViewMatrix = glm::mat4(1);

	// projection properties
	unsigned int m_screenWidth = 1280;
	unsigned int m_screenHeight = 720;
	float m_nearPlane = 0.1f;
	float m_farPlane = 1000.0f;
	float m_fieldOfView = 45.0f;
};
#endif