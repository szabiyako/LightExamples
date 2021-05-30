#include "DebugMenu.h"

UI::DebugMenu::DebugMenu(
	bool& isOpen,
	const DebugMenuDataRef &dataRef)
	: m_isOpen(isOpen),
	  m_dataRef(dataRef),
	  m_console(m_showConsole)
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

void UI::DebugMenu::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
{
	m_console.process(windowWidth, windowHeight, enableKeysInput);
	if (m_showUiDebug)
		ImGui::ShowMetricsWindow(&m_showUiDebug);

	if (!m_isOpen)
		return;

	ImGui::SetNextWindowPos(ImVec2(165, 40), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(160, 145), ImGuiCond_Always);
	ImGui::Begin("Debug menu", &m_isOpen, m_windowFlags);
	ImGui::Checkbox("Show console", &m_showConsole);
	ImGui::Checkbox("Show ui debug", &m_showUiDebug);
	ImGui::Checkbox("Show fps counter", &m_dataRef.enableFPScounter);
	ImGui::Checkbox("Show cursor", &m_dataRef.enableCursor);
	ImGui::End();
}
