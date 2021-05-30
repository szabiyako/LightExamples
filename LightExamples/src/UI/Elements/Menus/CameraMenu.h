#pragma once

#include "UI/Element.h"
#include "UI/MenuDataRef/CameraMenuDataRef.h"

#include "glm/glm.hpp"

namespace UI {
class CameraMenu : public Element
{
public:
	CameraMenu(bool &isOpen, const CameraMenuDataRef &dataRef);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
private:
	void loadCameraData();
	void saveCameraData();
	ImGuiWindowFlags m_windowFlags = 0;

	bool &m_isOpen;
	CameraMenuDataRef m_dataRef;

	glm::vec3 m_pos;
	float m_yaw;
	float m_pitch;
	float m_fov;
	float m_near;
	float m_far;
};
}

