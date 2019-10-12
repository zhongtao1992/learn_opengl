#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera{
public:
	enum CameraDir
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
public:
	Camera(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);

	void updateSpeed(const float& fSpeed) { m_moveSpeed = fSpeed; }

	void moveCamera(const CameraDir& dir);

	void mouseEvent(const float& xoffset, const float& yoffset);

	void scrollEvent(const float& yoffset);

	glm::mat4 getViewMatrix();

	glm::mat4 getLookAt();

	float getZoom() { return m_fov; };

	glm::vec3 getPosition() { return m_cameraPos; }

	glm::vec3 getFront() { return m_cameraFront; }
private:
	void updateVector();

private:
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;
	glm::vec3 m_worldUp;
	float m_moveSpeed;
	float m_pitch;
	float m_yaw;
	float m_fov;
};