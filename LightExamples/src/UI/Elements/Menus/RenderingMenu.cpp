#include "RenderingMenu.h"

UI::RenderingMenu::RenderingMenu(bool& isOpen, const RenderingMenuDataRef &dataRef)
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
}

void UI::RenderingMenu::process(const int windowWidth, const int windowHeight, bool& enableKeysInput)
{
	if (!m_isOpen)
		return;

	ImGui::SetNextWindowPos(ImVec2(0, 215), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(240, 342), ImGuiCond_Always);
	ImGui::Begin("Rendering", &m_isOpen, m_windowFlags);
	
	ImGui::Text("Rendering type");
	ImGui::SameLine();
	if (ImGui::Button(renderType.c_str()))
		ImGui::OpenPopup("renderingTypePopup");
	std::vector<std::string> renderTypes = { "Default", "Deferred", "Ray Tracing" };
	if (ImGui::BeginPopup("renderingTypePopup"))
	{
		for (const std::string& name : renderTypes)
			if (ImGui::Selectable(name.c_str()))
				renderType = name;
		ImGui::EndPopup();
	}
	ImGui::Text("Cube map");
	ImGui::Text("Enable grid");
	ImGui::Checkbox("VSync", &m_dataRef.enableVSync);
	ImGui::Text("FPS cup");

	ImGui::End();
}
