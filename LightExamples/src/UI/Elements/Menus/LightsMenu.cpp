#include "LightsMenu.h"

UI::LightsMenu::LightsMenu(
	bool& isOpen,
	const LightsMenuDataRef& dataRef)
	: m_isOpen(isOpen),
	  m_dataRef(dataRef)
{
	//m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	//m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	//m_windowFlags |= ImGuiWindowFlags_MenuBar;
	//m_windowFlags |= ImGuiWindowFlags_NoMove;
	//m_windowFlags |= ImGuiWindowFlags_NoResize;
	//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	//m_windowFlags |= ImGuiWindowFlags_NoNav;
	//m_windowFlags |= ImGuiWindowFlags_NoBackground;
	//m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void UI::LightsMenu::process(const int windowWidth, const int windowHeight, bool& enableKeysInput)
{
	const size_t nObjects = m_lightMenus.size();
	for (size_t objIndex = 0; objIndex < nObjects; ++objIndex) {
		// send ref to data
		bool isOpen = m_showObjectMenu[objIndex];
		Drawable* drawable = &m_dataRef.drawableVector[objIndex];
		glm::mat4x4* modelMatrix = &m_dataRef.modelMatrixVector[objIndex];

		m_lightMenus[objIndex].setDataPtrs(
			&isOpen,
			drawable,
			modelMatrix);
		m_lightMenus[objIndex].process(windowWidth, windowHeight, enableKeysInput);
		m_showObjectMenu[objIndex] = isOpen;
	}

	if (!m_isOpen)
		return;

	ImGui::SetNextWindowPos(ImVec2(480, 445), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(160, 400), ImGuiCond_Once);
	ImGui::Begin("Lights menu", &m_isOpen, m_windowFlags);
	if (ImGui::Button("Add Object")) {
		m_showObjectMenu.push_back(false);
		m_dataRef.modelMatrixVector.push_back(glm::mat4(1.f));
		m_dataRef.drawableVector.push_back(Drawable());
		if (nObjects != 0) {
			ImVec2 newPos = m_lightMenus[nObjects - 1].getWindowPos();
			newPos.x += 195;
			m_lightMenus.push_back(LightMenu(newPos));
		}
		else
			m_lightMenus.push_back(LightMenu());
	}
	int indexToDelete = -1;
	for (size_t objIndex = 0; objIndex < nObjects; ++objIndex) {
		bool isOpen = m_showObjectMenu[objIndex];
		const std::string checkBoxLabel = m_lightMenus[objIndex].getName() + "##" + m_lightMenus[objIndex].getUniqueName();
		ImGui::Checkbox(checkBoxLabel.c_str(), &isOpen);
		m_showObjectMenu[objIndex] = isOpen;
		ImGui::SameLine();
		if (ImGui::Button(std::string("remove##" + std::to_string(objIndex)).c_str())) {
			indexToDelete = objIndex;
		}
	}
	if (indexToDelete != -1) {
		m_showObjectMenu.erase(m_showObjectMenu.begin() + indexToDelete);
		m_lightMenus.erase(m_lightMenus.begin() + indexToDelete);
		m_dataRef.modelMatrixVector.erase(m_dataRef.modelMatrixVector.begin() + indexToDelete);
		delete m_dataRef.drawableVector[indexToDelete].drawableData;
		delete m_dataRef.drawableVector[indexToDelete].renderPipeline;
		m_dataRef.drawableVector.erase(m_dataRef.drawableVector.begin() + indexToDelete);
	}

	ImGui::End();
}
