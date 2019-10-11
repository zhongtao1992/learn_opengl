#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up){
	m_cameraPos = pos;
	m_cameraFront = front;
	m_worldUp = up;
	m_moveSpeed = 0.0f;
	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_fov = 45.0f;

	updateVector();
}

void Camera::moveCamera(const CameraDir& dir){
	switch (dir)
	{
	case CameraDir::FORWARD:
		m_cameraPos += m_cameraFront * m_moveSpeed;
		break;
	case CameraDir::BACKWARD:
		m_cameraPos -= m_cameraFront * m_moveSpeed;
		break;
	case CameraDir::LEFT:
		m_cameraPos -= m_cameraRight * m_moveSpeed;
		break;
	case CameraDir::RIGHT:
		m_cameraPos += m_cameraRight * m_moveSpeed;
		break;
	default:
		break;
	}
}

void Camera::mouseEvent(const float& xoffset, const float& yoffset){
	m_pitch += yoffset;
	m_yaw += xoffset;

	// constraint
	if (m_pitch > 89.0f) {
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f) {
		m_pitch = -89.0f;
	}
	updateVector();
}

void Camera::scrollEvent(const float& yoffset){
	if (m_fov >= 1.0f && m_fov <= 45.0f) {
		m_fov -= yoffset;
	}
	if (m_fov <= 1.0f) {
		m_fov = 1.0f;
	}
	if (m_fov >= 45.0f) {
		m_fov = 45.0f;
	}
}

glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::mat4 Camera::getLookAt(){
	glm::vec3 dir = glm::normalize(m_cameraPos - m_cameraFront);
	glm::vec3 right = glm::normalize(glm::cross(m_cameraUp, dir));
	glm::vec3 up = glm::cross(dir, right);
	
	glm::mat4 trans = glm::mat4(1.0f);
	trans[3][0] = -m_cameraPos.x;
	trans[3][1] = -m_cameraPos.y;
	trans[3][2] = -m_cameraPos.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = right.x;
	rotation[1][0] = right.y;
	rotation[2][0] = right.z;
	rotation[0][1] = up.x;
	rotation[1][1] = up.y;
	rotation[2][1] = up.z;
	rotation[0][2] = dir.x;
	rotation[1][2] = dir.y;
	rotation[2][2] = dir.z;

	return rotation * trans;
}

void Camera::updateVector(){
	float _pitch = glm::radians(m_pitch);
	float _yaw = glm::radians(m_yaw);
	// calcute direction
	glm::vec3 front;
	front.x = cos(_yaw) * cos(_pitch);
	front.y = sin(_pitch);
	front.z = sin(_yaw) * cos(_pitch);
	m_cameraFront = glm::normalize(front);

	// camera rigth
	m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
	// camera up
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}