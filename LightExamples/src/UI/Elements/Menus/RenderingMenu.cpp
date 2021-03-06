#include "RenderingMenu.h"

#include "Utils/Console.h"

#include "LoadableData/Image/Tools/Import/Import.h"

UI::RenderingMenu::RenderingMenu(bool& isOpen, const RenderingMenuDataRef &dataRef)
	: m_isOpen(isOpen),
	  m_dataRef(dataRef),
	  m_cubeMapImageFileNames(std::vector<std::string>(6, "None"))
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

	ImGui::SetNextWindowPos(ImVec2(0, 260), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320, 342), ImGuiCond_Always);
	ImGui::Begin("Rendering", &m_isOpen, m_windowFlags);
	
	ImGui::Text("Rendering type");
	ImGui::SameLine();
	if (ImGui::Button(getRenderTypeAsString(m_dataRef.renderingType).c_str()))
		ImGui::OpenPopup("renderingTypePopup");
	if (ImGui::BeginPopup("renderingTypePopup"))
	{
		std::vector<RenderingType> renderingTypes = { RenderingType::DEFAULT, RenderingType::DEFERRED, RenderingType::RAYTRACING };
		for (const RenderingType& renderingType : renderingTypes)
			if (ImGui::Selectable(getRenderTypeAsString(renderingType).c_str()))
				m_dataRef.renderingType = renderingType;
		ImGui::EndPopup();
	}

	if (m_dataRef.renderingType == RenderingType::RAYTRACING) {
		int reflections = m_dataRef.rayTracer.getMaxRays();
		ImGui::DragInt("Reflections", &reflections, 1.0f, 1, 40);
		if (reflections < 1)
			reflections = 1;
		else if (reflections > 40)
			reflections = 40;
		m_dataRef.rayTracer.setMaxRays(reflections);
		float scale = m_dataRef.rayTracer.getResolutionScale();
		ImGui::DragFloat("Scale", &scale, 0.01f, 0.1f, 2.f);
		m_dataRef.rayTracer.setResolutionScale(scale);
		bool interpolate = m_dataRef.rayTracer.getInterpolateTextures();
		ImGui::Checkbox("Interpolate", &interpolate);
		m_dataRef.rayTracer.setInterpolateTextures(interpolate);
		int maxFrames = m_dataRef.rayTracer.getMaxFrames();
		ImGui::DragInt("Rays per pixel", &maxFrames, 1.f, 2, 50000);
		if (maxFrames > 1)
			m_dataRef.rayTracer.setMaxFrames(maxFrames);
		ImGui::NewLine();
	}
	if (ImGui::CollapsingHeader("Background")) {
		backgroundHeader();
	}
	if (ImGui::CollapsingHeader("VSync##Header")) {
		ImGui::Checkbox("VSync", &m_dataRef.enableVSync);
	}
	if (ImGui::CollapsingHeader("SSAO##Header")) {
		if (m_dataRef.renderingType != RenderingType::DEFERRED) {
			ImGui::Text("Works only with Deferred render");
		}
		else {
			ImGui::Checkbox("SSAO", &m_dataRef.enableSSAO);
		}
	}

	if (ImGuiFileDialog::Instance()->Display("ChooseImageCubeMap_Right"))
		loadCubeMapTexturefileDialog(0);
	if (ImGuiFileDialog::Instance()->Display("ChooseImageCubeMap_Left"))
		loadCubeMapTexturefileDialog(1);
	if (ImGuiFileDialog::Instance()->Display("ChooseImageCubeMap_Bottom"))
		loadCubeMapTexturefileDialog(2);
	if (ImGuiFileDialog::Instance()->Display("ChooseImageCubeMap_Top"))
		loadCubeMapTexturefileDialog(3);
	if (ImGuiFileDialog::Instance()->Display("ChooseImageCubeMap_Front"))
		loadCubeMapTexturefileDialog(4);
	if (ImGuiFileDialog::Instance()->Display("ChooseImageCubeMap_Back"))
		loadCubeMapTexturefileDialog(5);

	ImGui::End();
}

void UI::RenderingMenu::backgroundHeader()
{
	ImGui::Checkbox("use Cube map", &m_useCubeMap);
	if (m_useCubeMap) {
		ImGui::Text("Right");
		ImGui::SameLine();
		if (ImGui::Button(m_cubeMapImageFileNames[0].c_str()))
			ImGuiFileDialog::Instance()->OpenDialog("ChooseImageCubeMap_Right", "Choose a Right texture", ".jpg,.jpeg,.png", ".");
		ImGui::Text("Left");
		ImGui::SameLine();
		if (ImGui::Button(m_cubeMapImageFileNames[1].c_str()))
			ImGuiFileDialog::Instance()->OpenDialog("ChooseImageCubeMap_Left", "Choose a Left texture", ".jpg,.jpeg,.png", ".");
		ImGui::Text("Bottom");
		ImGui::SameLine();
		if (ImGui::Button(m_cubeMapImageFileNames[2].c_str()))
			ImGuiFileDialog::Instance()->OpenDialog("ChooseImageCubeMap_Bottom", "Choose a Bottom texture", ".jpg,.jpeg,.png", ".");
		ImGui::Text("Top");
		ImGui::SameLine();
		if (ImGui::Button(m_cubeMapImageFileNames[3].c_str()))
			ImGuiFileDialog::Instance()->OpenDialog("ChooseImageCubeMap_Top", "Choose a Top texture", ".jpg,.jpeg,.png", ".");
		ImGui::Text("Front");
		ImGui::SameLine();
		if (ImGui::Button(m_cubeMapImageFileNames[4].c_str()))
			ImGuiFileDialog::Instance()->OpenDialog("ChooseImageCubeMap_Front", "Choose a Front texture", ".jpg,.jpeg,.png", ".");
		ImGui::Text("Back");
		ImGui::SameLine();
		if (ImGui::Button(m_cubeMapImageFileNames[5].c_str()))
			ImGuiFileDialog::Instance()->OpenDialog("ChooseImageCubeMap_Back", "Choose a Back texture", ".jpg,.jpeg,.png", ".");
		if (ImGui::Button("Load default Cube map")) {
			std::string errorMessage;
			ImageTools::Import::fromFile(m_dataRef.skyboxImages[0], "res/textures/Skybox/right.jpg", errorMessage);
			ImageTools::Import::fromFile(m_dataRef.skyboxImages[1], "res/textures/Skybox/left.jpg", errorMessage);
			ImageTools::Import::fromFile(m_dataRef.skyboxImages[2], "res/textures/Skybox/bottom.jpg", errorMessage);
			ImageTools::Import::fromFile(m_dataRef.skyboxImages[3], "res/textures/Skybox/top.jpg", errorMessage);
			ImageTools::Import::fromFile(m_dataRef.skyboxImages[4], "res/textures/Skybox/front.jpg", errorMessage);
			ImageTools::Import::fromFile(m_dataRef.skyboxImages[5], "res/textures/Skybox/back.jpg", errorMessage);
			m_cubeMapImageFileNames[0] = "right.jpg";
			m_cubeMapImageFileNames[1] = "left.jpg";
			m_cubeMapImageFileNames[2] = "bottom.jpg";
			m_cubeMapImageFileNames[3] = "top.jpg";
			m_cubeMapImageFileNames[4] = "front.jpg";
			m_cubeMapImageFileNames[5] = "back.jpg";
			applyCubeMap();
		}
	}
	else {
		ImGui::ColorEdit3("Color", m_cubeMapColor);
	}
	if (ImGui::Button("Apply"))
	{
		applyCubeMap();
	}
}

void UI::RenderingMenu::loadCubeMapTexturefileDialog(const size_t textureIndex)
{
	if (ImGuiFileDialog::Instance()->IsOk())
	{
		const std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
		const std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
		// action
		std::string errorMessage;
		ImageTools::Import::fromFile(m_dataRef.skyboxImages[textureIndex], filePath, errorMessage);
		if (!errorMessage.empty()) {
			Console::print("Failed to load image \"" + filePath + "\"\n");
		}
		else
			m_cubeMapImageFileNames[textureIndex] = fileName;
	}
	ImGuiFileDialog::Instance()->Close();
}

void UI::RenderingMenu::applyCubeMap()
{
	if (m_useCubeMap) {
		bool isOk = true;
		const int w = m_dataRef.skyboxImages[0].width;
		const int h = m_dataRef.skyboxImages[0].height;
		for (size_t i = 0; (i < 6) && isOk; ++i) {
			if (m_dataRef.skyboxImages[i].isEmpty()
				|| (w != m_dataRef.skyboxImages[i].width)
				|| (h != m_dataRef.skyboxImages[i].height))
				isOk = false;
		}
		if (isOk)
			m_dataRef.skyboxCubeMap.loadFromImages(m_dataRef.skyboxImages);
		else {
			Console::print("Failed to create skybox Cubemap\n");
		}
	}
	else {
		m_dataRef.skyboxCubeMap.loadFromColor(glm::vec3(m_cubeMapColor[0], m_cubeMapColor[1], m_cubeMapColor[2]));
	}
}

std::string UI::RenderingMenu::getRenderTypeAsString(const RenderingType& renderingType)
{
	switch (renderingType)
	{
	case RenderingType::DEFAULT: return "Default";
	case RenderingType::DEFERRED: return "Deferred";
	case RenderingType::RAYTRACING: return "Ray Tracing";
	}
	return "UNKNOWN";
}
