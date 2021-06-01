#include "LightMenu.h"

#include "LoadableData/ObjData/Tools/Import.h"
#include "Utils/Console.h"
#include "Drawable/DrawableData/Default/Default.h"
#include "Drawable/RenderPipeline/Default/Default.h"

#include "glm/gtc/matrix_transform.hpp"


UI::LightMenu::LightMenu(const ImVec2& windowPos)
	: m_windowPos(windowPos)
{
	//m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	//m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	//m_windowFlags |= ImGuiWindowFlags_MenuBar;
	//m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoResize;
	//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	//m_windowFlags |= ImGuiWindowFlags_NoNav;
	//m_windowFlags |= ImGuiWindowFlags_NoBackground;
	//m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	m_uniqueName = getNextUniqueName();
	m_name = "New light";
}

void UI::LightMenu::process(const int windowWidth, const int windowHeight, bool& enableKeysInput)
{
	if (!isDataPtrsSetup())
		return;

	if (!(*m_isOpen))
		return;

	ImGui::SetNextWindowPos(m_windowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(190, 400), ImGuiCond_Once);

	ImGui::Begin(std::string(m_name + "##" + m_uniqueName).c_str(), m_isOpen, m_windowFlags);
	m_windowPos = ImGui::GetWindowPos();

	std::string loadObjectLabel = m_fileName;

	const bool canBeDrawn = m_lightSrc->getDrawable().canBeDrawn();
	if (!canBeDrawn)
		loadObjectLabel = "Select";
	ImGui::Text("Name");
	ImGui::SameLine();
	ImGuiInputTextFlags flags = 0;
	flags |= ImGuiInputTextFlags_EnterReturnsTrue;
	if (ImGui::InputText("", &m_newName, flags))
		m_name = m_newName;
	if (ImGui::IsItemActive()) {
		enableKeysInput = false;
	}
	else {
		m_newName = m_name;
	}
	ImGui::Text("Light type");
	ImGui::SameLine();
	if (ImGui::Button(loadObjectLabel.c_str())) {
		//ImGuiFileDialog::Instance()->Close();
		//ImGui::SetNextWindowPos(ImVec2(m_windowPos.x + 40, m_windowPos.y + 70), ImGuiCond_Always);
		//ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
		//ImGuiFileDialog::Instance()->OpenDialog(m_uniqueName, "Choose File", ".obj", ".");
	}

	if (canBeDrawn) {
		if (ImGui::CollapsingHeader("Draw"))
		{
			bool isVisible = m_lightSrc->isVisible();
			ImGui::Checkbox("Visible", &isVisible);
			const glm::vec3 currentColor = m_lightSrc->getColor();
			float color[3] = { currentColor.x, currentColor.y, currentColor.z };
			ImGui::ColorEdit3("Color", color);

			m_lightSrc->setVisible(isVisible);
			m_lightSrc->setColor(glm::vec3(color[0], color[1], color[2]));

		}
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			glm::vec3 &position = m_lightSrc->getPosRef();
			ImGui::DragFloat("x##pos", &position.x, 0.05f);
			ImGui::DragFloat("y##pos", &position.y, 0.05f);
			ImGui::DragFloat("z##pos", &position.z, 0.05f);
			glm::vec3 &rotations = m_lightSrc->getRotationRef();
			ImGui::Text("Rotation");
			ImGui::DragFloat("x##rot", &rotations.x, 0.5f);
			ImGui::DragFloat("y##rot", &rotations.y, 0.5f);
			ImGui::DragFloat("z##rot", &rotations.z, 0.5f);
			float &scale = m_lightSrc->getScaleRef();
			ImGui::DragFloat("scale", &scale, 0.05f);
			if (rotations.x > 180)
				rotations.x -= 360;
			else if (rotations.x < -180)
				rotations.x += 360;
		
			if (rotations.y > 180)
				rotations.y -= 360;
			else if (rotations.y < -180)
				rotations.y += 360;
		
			if (rotations.z > 180)
				rotations.z -= 360;
			else if (rotations.z < -180)
				rotations.z += 360;
			if (ImGui::Button("reset##transform")) {
				position = { 0.f, 0.f, 0.f };
				rotations = { 0.f, 0.f, 0.f };
				scale = 1.f;
			}
			m_lightSrc->updateModelMatrix();
			m_lightSrc->updateDirection();
		}
	}
	ImGui::End();
}

void UI::LightMenu::setDataPtrs(
	bool* isOpen,
	LightSrc* lightSrc)
{
	m_isOpen = isOpen;
	m_lightSrc = lightSrc;
}

bool UI::LightMenu::isDataPtrsSetup() const
{
	return (m_isOpen != nullptr)
		&& (m_lightSrc != nullptr);
}

std::string UI::LightMenu::getUniqueName() const
{
	return m_uniqueName;
}

std::string UI::LightMenu::getName() const
{
	return m_name;
}

ImVec2 UI::LightMenu::getWindowPos() const
{
	return m_windowPos;
}

std::string UI::LightMenu::getNextUniqueName()
{
	static int index = 0;
	return "Light " + std::to_string(index++);
}


