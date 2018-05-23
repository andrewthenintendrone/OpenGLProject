#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// An abstract camera class
class Camera
{
public:

	// constructors
	Camera();

	const glm::mat4 getProjectionMatrix();
	const glm::mat4 GetViewMatrix();
	const glm::mat4 getProjectionViewMatrix();

	void setPosition(const glm::vec3 position);
	void setRotation(const glm::vec3 rotation);

	const glm::vec3 getPosition();
	const glm::vec3 getRotation();

private:

	void updateTransform();

	// Camera Attributes
	glm::vec4 m_worldPos = glm::vec4(0, 0, 0, 1); // position in world space
	glm::vec4 m_right = glm::vec4(1, 0, 0, 1); // right vector
	glm::vec4 m_up = glm::vec4(0, 1, 0, 1); // up vector
	glm::vec4 m_front = glm::vec4(0, 0, 1, 1); // forward vector
	glm::vec4 m_worldUp; // world up vector

	glm::mat4 m_viewMatrix = glm::mat4(1);
	glm::mat4 m_projectionMatrix = glm::mat4(1);

	// projection properties
	unsigned int m_screenWidth = 1280;
	unsigned int m_screenHeight = 720;
	float m_nearPlane = 0.1f;
	float m_farPlane = 1000.0f;
	float m_fieldOfView = 60.0f;
};
#endif