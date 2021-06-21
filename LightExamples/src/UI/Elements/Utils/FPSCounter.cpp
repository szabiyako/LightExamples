#include "FPSCounter.h"

UI::FPSCounter::FPSCounter(bool &show)
	: m_show(show)
{
	m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	//m_windowFlags |= ImGuiWindowFlags_MenuBar;
	m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoResize;
	m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	m_windowFlags |= ImGuiWindowFlags_NoNav;
	//m_windowFlags |= ImGuiWindowFlags_NoBackground;
	m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void UI::FPSCounter::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
{
	if (!m_show)
		return;
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(190, 29), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.3);
	ImGui::Begin("FPS", NULL, m_windowFlags);
	ImGui::Text("%.0f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	//ImGui::Text("%.0f FPS (%.3f ms/frame)", 1.0f / m_DeltaTime, double(m_DeltaTime * 1000));
	ImGui::End();
}
