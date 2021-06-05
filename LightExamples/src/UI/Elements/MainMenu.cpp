#include "MainMenu.h"

#include "Camera.h"

UI::MainMenu::MainMenu(const DataRef& dataRef)
	: m_debugMenu(m_isDebugMenuOpened,
				  dataRef.debugMenuDataRef),
	  m_cameraMenu(m_isCameraMenuOpened,
				   dataRef.cameraMenuDataRef),
	  m_renderingMenu(m_isRenderingMenuOpened,
					  dataRef.renderingMenuDataRef),
	  m_objectsMenu(m_isObjectsMenuOpened,
		            dataRef.objectsMenuDataRef),
	  m_lightsMenu(m_isLightsMenuOpened,
				   dataRef.lightsMenuDataRef)
{
	//m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	//m_windowFlags |= ImGuiWindowFlags_MenuBar;
	//m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoResize;
	//m_windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
	//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	//m_windowFlags |= ImGuiWindowFlags_NoNav;
	//m_windowFlags |= ImGuiWindowFlags_NoBackground;
	//m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void UI::MainMenu::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
{
	ImGui::SetNextWindowPos(ImVec2(0, 40), ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(160, 145), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(160, 170), ImGuiCond_Always);
	ImGui::Begin("Menu", NULL, m_windowFlags);
	ImGui::Checkbox("Debug menu", &m_isDebugMenuOpened);
	m_debugMenu.process(windowWidth, windowHeight, enableKeysInput);
	ImGui::Checkbox("Camera", &m_isCameraMenuOpened);
	m_cameraMenu.process(windowWidth, windowHeight, enableKeysInput);
	ImGui::Checkbox("Rendering", &m_isRenderingMenuOpened);
	m_renderingMenu.process(windowWidth, windowHeight, enableKeysInput);
	ImGui::Checkbox("Objects in scene", &m_isObjectsMenuOpened);
	m_objectsMenu.process(windowWidth, windowHeight, enableKeysInput);
	ImGui::Checkbox("Lights in scene", &m_isLightsMenuOpened);
	m_lightsMenu.process(windowWidth, windowHeight, enableKeysInput);

	//Temp
	ImGui::Checkbox("Demo window", &m_isDemoWindowOpened);
	if (m_isDemoWindowOpened)
		ImGui::ShowDemoWindow(&m_isDemoWindowOpened);
	ImGui::End();
}