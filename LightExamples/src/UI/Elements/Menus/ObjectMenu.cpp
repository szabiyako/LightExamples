#include "ObjectMenu.h"

#include "LoadableData/ObjData/Tools/Import.h"
#include "Utils/Console.h"
#include "Drawable/DrawableData/Geometry/Geometry.h"
#include "Drawable/RenderPipeline/Basic/BasicRenderPipeline.h"

#include "glm/gtc/matrix_transform.hpp"

std::string getNextUniqueName()
{
	static int index = 0;
	return "Object " + std::to_string(index++);
}

UI::ObjectMenu::ObjectMenu(const ImVec2& windowPos)
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
	m_name = "New object";
}

void UI::ObjectMenu::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
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

	const bool canBeDrawn = m_drawable->canBeDrawn();
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
	ImGui::Text("Obj file");
	ImGui::SameLine();
	if (ImGui::Button(loadObjectLabel.c_str())) {
		ImGuiFileDialog::Instance()->Close();
		ImGui::SetNextWindowPos(ImVec2(m_windowPos.x + 40, m_windowPos.y + 70), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
		ImGuiFileDialog::Instance()->OpenDialog(m_uniqueName, "Choose File", ".obj", ".");
	}

	if (canBeDrawn) {
		RenderPipeline::BasicRenderPipeline* base = dynamic_cast<RenderPipeline::BasicRenderPipeline*>(m_drawable->renderPipeline);
		if (base != nullptr) {
			if (ImGui::CollapsingHeader("Draw"))
			{
				ImGui::Checkbox("Visible", &m_isVisible);
				ImGui::Checkbox("Cull faces", &m_cullFaces);
				ImGui::ColorEdit3("Color", m_color);
				base->setCullFace(m_cullFaces);
				base->setColor(glm::vec3(m_color[0], m_color[1], m_color[2]));
				m_drawable->setVisible(m_isVisible);
			}
			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::Text("Position");
				ImGui::DragFloat("x##pos", &m_position.x, 0.05f);
				ImGui::DragFloat("y##pos", &m_position.y, 0.05f);
				ImGui::DragFloat("z##pos", &m_position.z, 0.05f);
				ImGui::Text("Rotation");
				ImGui::DragFloat("x##rot", &m_rotation.x, 0.05f);
				ImGui::DragFloat("y##rot", &m_rotation.y, 0.05f);
				ImGui::DragFloat("z##rot", &m_rotation.z, 0.05f);
				ImGui::DragFloat("scale", &m_scale, 0.05f);
				glm::mat4x4 modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
				modelMatrix = glm::rotate(modelMatrix, m_rotation.x, glm::vec3(1.f, 0.f, 0.f));
				modelMatrix = glm::rotate(modelMatrix, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
				modelMatrix = glm::rotate(modelMatrix, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
				glm::mat4x4 undoRotated = glm::inverse(modelMatrix);
				*m_modelMatrix = glm::translate(modelMatrix, glm::vec3(undoRotated * glm::vec4(m_position, 1.f)));
				if (ImGui::Button("reset##transform")) {
					m_position = { 0.f, 0.f, 0.f };
					m_rotation = { 0.f, 0.f, 0.f };
					m_scale = 1.f;
				}
			}
		}
	}

	// display
	if (ImGuiFileDialog::Instance()->Display(m_uniqueName))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			const std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			ObjData objData;
			std::string errorMessage;
			ObjDataTools::Import::fromFile(objData, errorMessage, filePathName);
			const bool isLoaded = errorMessage.empty();
			if (!isLoaded){
				Console::print("Failed to load \"" + filePathName + "\"" + errorMessage + '\n');
			}
			else {
				Console::print("Loaded \"" + filePathName + "\"\n");
				m_fileName = fileName;
				if (m_name == "New object")
					m_name = fileName;
				m_loadableData->objData = objData;
				DrawableData::Geometry *geom = new DrawableData::Geometry(objData);
				if (m_drawable->drawableData != nullptr)
					delete m_drawable->drawableData;
				m_drawable->drawableData = geom;
				RenderPipeline::BasicRenderPipeline* base = new RenderPipeline::BasicRenderPipeline();
				if (m_drawable->renderPipeline != nullptr)
					delete m_drawable->renderPipeline;
				m_drawable->renderPipeline = base;
			}
		}
		// close
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::End();
}

void UI::ObjectMenu::setDataPtrs(
	bool* isOpen,
	LoadableData* loadableData,
	Drawable* drawable,
	glm::mat4x4 *modelMatrix)
{
	m_isOpen = isOpen;
	m_loadableData = loadableData;
	m_drawable = drawable;
	m_modelMatrix = modelMatrix;
}

bool UI::ObjectMenu::isDataPtrsSetup() const
{
	return (m_isOpen != nullptr)
	       && (m_loadableData != nullptr)
	       && (m_drawable != nullptr)
		   && (m_modelMatrix != nullptr);
}

std::string UI::ObjectMenu::getUniqueName() const
{
	return m_uniqueName;
}

std::string UI::ObjectMenu::getName() const
{
	return m_name;
}

ImVec2 UI::ObjectMenu::getWindowPos() const
{
	return m_windowPos;
}


