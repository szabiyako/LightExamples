#include "CameraMenu.h"

#include "Camera.h"

UI::CameraMenu::CameraMenu(bool &isOpen, const CameraMenuDataRef &dataRef)
	: m_isOpen(isOpen),
	  m_dataRef(dataRef)
{
	//m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	//m_windowFlags |= ImGuiWindowFlags_MenuBar;
	//m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoResize;
	//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	//m_windowFlags |= ImGuiWindowFlags_NoNav;
	//m_windowFlags |= ImGuiWindowFlags_NoBackground;
	//m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	loadCameraData();
}

void UI::CameraMenu::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
{
	if (!m_isOpen)
		return;

	loadCameraData();

	ImGui::SetNextWindowPos(ImVec2(330, 40), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(145, 384), ImGuiCond_Always);
	ImGui::Begin("Camera", &m_isOpen, m_windowFlags);
	ImGui::DragFloat("speed", &m_dataRef.cameraSpeed, 0.05f);
	if (ImGui::Button("reset##speed", ImVec2(130, 18))) {
		m_dataRef.cameraSpeed = 40.f;
	}
	ImGui::DragFloat("sens", &m_dataRef.cameraSensitivity, 0.001f);
	if (ImGui::Button("reset##sensitivity", ImVec2(130, 18))) {
		m_dataRef.cameraSensitivity = 0.05f;
	}
	ImGui::Text("View params:");
	ImGui::DragFloat("x", &m_pos.x, 0.05f);
	ImGui::DragFloat("y", &m_pos.y, 0.05f);
	ImGui::DragFloat("z", &m_pos.z, 0.05f);
	ImGui::DragFloat("yaw", &m_yaw, 0.05f);
	ImGui::DragFloat("pitch", &m_pitch, 0.05f);
	if (ImGui::Button("reset##view", ImVec2(130, 18))) {
		m_pos = glm::vec3(0.f, 0.f, 0.f);
		m_yaw = -90.f;
		m_pitch = 0.f;
	}
	ImGui::Text("Projection params:");
	ImGui::DragFloat("fov", &m_fov, 0.05f);
	ImGui::DragFloat("near", &m_near, 0.01f);
	ImGui::DragFloat("far", &m_far, 0.1f);
	if (ImGui::Button("reset##proj", ImVec2(130, 18))) {
		m_fov = 45.f;
		m_near = 0.1f;
		m_far = 1000.f;
	}

	ImGui::End();

	saveCameraData();
}

void UI::CameraMenu::loadCameraData()
{
	m_pos = m_dataRef.camera.getPos();
	m_yaw = m_dataRef.camera.getYaw();
	m_pitch = m_dataRef.camera.getPitch();
	m_fov = m_dataRef.camera.getFOV();
	m_near = m_dataRef.camera.getNearSize();
	m_far = m_dataRef.camera.getFarSize();
}

void UI::CameraMenu::saveCameraData()
{
	m_dataRef.camera.setPos(m_pos);
	m_dataRef.camera.setYaw(m_yaw);
	m_dataRef.camera.setPitch(m_pitch);
	m_dataRef.camera.setFOV(m_fov);
	m_dataRef.camera.setNearSize(m_near);
	m_dataRef.camera.setFarSize(m_far);
}
