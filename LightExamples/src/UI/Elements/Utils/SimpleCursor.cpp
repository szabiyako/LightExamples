#include "SimpleCursor.h"

UI::SimpleCursor::SimpleCursor(bool &show)
	: m_show(show)
{
	m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	//m_windowFlags |= ImGuiWindowFlags_MenuBar;
	m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoResize;
	m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	m_windowFlags |= ImGuiWindowFlags_NoNav;
	m_windowFlags |= ImGuiWindowFlags_NoBackground;
	m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void UI::SimpleCursor::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
{
	if (!m_show)
		return;

	ImGui::SetNextWindowPos(ImVec2(float(windowWidth) / 2 - 12, float(windowHeight) / 2 - 16), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(20, 20), ImGuiCond_Always);
	ImGui::Begin("Cursor", NULL, m_windowFlags);
	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 0.9f), "o");
	ImGui::End();
}
