#pragma once

#include "glm/glm.hpp"

#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"

class Camera
{
private:
	//Base for View
	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;

	//View matrix
	glm::mat4 m_viewMatrix;

	//For projection
	float m_fov;          // in degrees
	float m_aspectRatio;
	float m_nearSize;
	float m_farSize;

	//Projection matrix
	glm::mat4 m_projMatrix;

	//MouseRotation
	float m_lastX;
	float m_lastY;
	float m_yaw;
	float m_pitch;
	bool m_isFirstMouseMove;

	bool m_isChangedView;
	bool m_isChangedProj;

	void clampYaw();
	void clampPitch();
public:
	//Full constructor
	Camera(const glm::vec3&   pos          = glm::vec3(0.0f, 0.0f, 3.5f),
		   const glm::vec3&   front        = glm::vec3(0.0f, 0.0f, -1.0f),
		   const glm::vec3&   up           = glm::vec3(0.0f, 1.0f, 0.0f),
		   const float&       yaw          = -90.f, 
		   const float&       pitch        = 0.f,
		   const float&       aspectratio  = 800.f/600.f,
		   const float&       fov          = 45.f,
		   const float&       near         = 0.1f,    //0.01f provides z-fighting at far distances, so better use 0.5f or 1.f or rewrite viewprojection matrix (or 32 bits z buffer - doesnt help)
		   const float&       far          = 1000.f
	);

	void rotateFromCursor(const double& xpos, const double& ypos, const float& sensitivity);
	void rotateFromJoystick(const float xpos, const float ypos, const float sensitivity);
	void resetStartCursorPos(const double& xpos, const double& ypos);
	void updateMatrices();


	//Inline Methods
	inline glm::mat4 getUImatrix()
	{
		return glm::ortho(-1.f, 1.f, -1.f / m_aspectRatio, 1.f / m_aspectRatio, 0.0f, 1.f);
	}

	//View
	inline void moveFront(const float& distance)
	{
		float k = glm::length(m_front) / glm::length(glm::vec2(m_front.x, m_front.z));
		m_pos += distance * k * glm::vec3(m_front.x, 0.f, m_front.z); // or just normalize
		m_isChangedView = true;
	}
	inline void moveFrontToView(const float& distance)
	{
		m_pos += distance * m_front;
		m_isChangedView = true;
	}
	inline void moveUp(const float& distance)
	{ 
		m_pos += distance * m_up;
		m_isChangedView = true;
	}
	inline void moveRight(const float& distance)
	{
		m_pos += distance * glm::normalize(glm::cross(m_front, m_up));
		m_isChangedView = true;
	}
	inline void setPos(const glm::vec3& pos)
	{
		m_pos = pos;
		m_isChangedView = true;
	}
	inline void setYaw(const float& yaw)
	{
		m_yaw = yaw;
		clampYaw();

		m_isChangedView = true;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		direction.y = sin(glm::radians(m_pitch));
		direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(direction);
	}
	inline void setPitch(const float& pitch)
	{
		m_pitch = pitch;
		clampPitch();

		m_isChangedView = true;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		direction.y = sin(glm::radians(m_pitch));
		direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(direction);
	}
	inline glm::vec3 getPos() const { return m_pos; }
	inline glm::vec3 getFront() const { return m_front; }
	inline float getPitch() const { return m_pitch; }
	inline float getYaw() const { return m_yaw; }
	inline float getFOV() const { return m_fov; }
	inline float getNearSize() const { return m_nearSize; }
	inline float getFarSize() const { return m_farSize; }
	inline glm::mat4 getView()
	{
		if (m_isChangedView)
		{
			m_isChangedView = false;
			return m_viewMatrix = glm::lookAt(m_pos, m_pos + m_front, m_up);
		}
		return m_viewMatrix;
	}

	//Proj
	inline void setNearSize(const float& near)
	{
		m_nearSize = near;
		m_isChangedProj = true;
	}
	inline void setFarSize(const float& far)
	{
		m_farSize = far;
		m_isChangedProj = true;
	}
	inline void setFOV(const float& fov)
	{
		m_fov = fov;
		m_isChangedProj = true;
	}
	inline void setAspectRatio(const float& aspectratio)
	{
		m_aspectRatio = aspectratio;
		m_isChangedProj = true;
	}
	inline glm::mat4 getProj()
	{
		if (m_isChangedProj)
		{
			m_isChangedProj = false;
			return m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearSize, m_farSize);
		}
		return m_projMatrix;
	}



	inline glm::mat4 getViewProj()
	{
		if (m_isChangedView)
		{
			m_isChangedView = false;
			m_viewMatrix = glm::lookAt(m_pos, m_pos + m_front, m_up);
		}
		if (m_isChangedProj)
		{
			m_isChangedProj = false;
			m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearSize, m_farSize);
		}
		return m_projMatrix * m_viewMatrix;
	}
};

