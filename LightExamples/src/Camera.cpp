#include "Camera.h"


void Camera::clampYaw()
{
	if (m_yaw > 180.f)
		m_yaw -= 360.f;
	else if (m_yaw < -180.f)
		m_yaw += 360.f;
}

void Camera::clampPitch()
{
	if (m_pitch > 89.f)
		m_pitch = 89.f;
	else if (m_pitch < -89.f)
		m_pitch = -89.f;
}

Camera::Camera(const glm::vec3 & pos, const glm::vec3 & front, const glm::vec3 & up, const float & yaw, const float & pitch,
			   const float& aspectratio, const float& fov, const float& near, const float& far)
	: m_isFirstMouseMove(true), m_lastX(0.f), m_lastY(0.f), m_isChangedView(false), m_isChangedProj(false),
	  m_pos(pos), m_front(front), m_up(up), m_yaw(yaw), m_pitch(pitch),
	  m_aspectRatio(aspectratio), m_fov(fov), m_nearSize(near), m_farSize(far)
{
	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_front, m_up);
	m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearSize, m_farSize);
}


void Camera::rotateFromCursor(const double& xpos, const double& ypos, const float& sensitivity)
{
	if (m_isFirstMouseMove)
	{
		m_lastX = (float)xpos;
		m_lastY = (float)ypos;
		m_isFirstMouseMove = false;
	}

	float xoffset = (float)xpos - m_lastX;
	float yoffset = m_lastY - (float)ypos;
	m_lastX = (float)xpos;
	m_lastY = (float)ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	clampYaw();
	clampPitch();

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(direction);

	m_isChangedView = true;
}

void Camera::rotateFromJoystick(const float xpos, const float ypos, const float sensitivity)
{
	float xoffset = xpos;
	float yoffset = -ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	clampYaw();
	clampPitch();

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(direction);

	m_isChangedView = true;
}

void Camera::resetStartCursorPos(const double & xpos, const double & ypos)
{
	m_lastX = (float)xpos;
	m_lastY = (float)ypos;
}

void Camera::updateMatrices()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(direction);

	m_isChangedView = true;
	m_isChangedProj = true;
}
