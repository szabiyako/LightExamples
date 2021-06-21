#include "RayTracingProgressBar.h"

UI::RayTracingProgressBar::RayTracingProgressBar(RayTracer& rayTracer, RenderingType& renderingType)
	: m_rayTracerRef(rayTracer),
	  m_renderingTypeRef(renderingType)
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

void UI::RayTracingProgressBar::process(const int windowWidth, const int windowHeight, bool& enableKeysInput)
{
	if (m_renderingTypeRef != RenderingType::RAYTRACING)
		return;

	ImGui::SetNextWindowPos(ImVec2(650.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(250.f, 29.f), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::Begin("RTprogressBarText", NULL, m_windowFlags);
	int currentFrame = m_rayTracerRef.getCurrentFrame();
	int maxFrames = m_rayTracerRef.getMaxFrames();
	ImGui::Text("%d/%d rays per pixel", currentFrame, maxFrames);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(400.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(250.f, 29.f), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::Begin("RTprogressBar", NULL, m_windowFlags);
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.f, 0.9f, 0.f, 1.f));
	ImGui::ProgressBar(currentFrame / float(maxFrames));
	ImGui::PopStyleColor();
	ImGui::End();
}
