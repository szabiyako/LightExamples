#include "ObjectMenu.h"

#include "LoadableData/ObjData/Tools/Import.h"
#include "Utils/Console.h"
#include "Drawable/DrawableData/Default/Default.h"
#include "Drawable/RenderPipeline/Default/Default.h"

#include "glm/gtc/matrix_transform.hpp"

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

	const bool canBeDrawn = m_model->getDrawable().canBeDrawn();
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
		if (ImGui::CollapsingHeader("Draw"))
		{
			bool isVisible = m_model->isVisible();
			ImGui::Checkbox("Visible", &isVisible);
			// Maby do this other way
			RenderPipeline::Default *defaultPipeline = dynamic_cast<RenderPipeline::Default*>(m_model->getDrawableRef().renderPipeline);
			bool cullFace = defaultPipeline->getCullFace();
			ImGui::Checkbox("Cull faces", &cullFace);
			const glm::vec3 currentColor = m_model->getColor();
			float color[3] = { currentColor.x, currentColor.y, currentColor.z };
			ImGui::ColorEdit3("Color", color);

			const bool hasUVs = !m_model->getLoadableDataRef().objData.indices.uv.empty();
			if (hasUVs) {
				bool a = true;
				ImGui::Checkbox("Has UVs", &a);
			}

			m_model->setVisible(isVisible);
			defaultPipeline->setCullFace(cullFace);
			m_model->setColor(glm::vec3(color[0], color[1], color[2]));
		}
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			glm::vec3& position = m_model->getPosRef();
			ImGui::DragFloat("x##pos", &position.x, 0.05f);
			ImGui::DragFloat("y##pos", &position.y, 0.05f);
			ImGui::DragFloat("z##pos", &position.z, 0.05f);
			glm::vec3& rotations = m_model->getRotationRef();
			ImGui::Text("Rotation");
			ImGui::DragFloat("x##rot", &rotations.x, 0.5f);
			ImGui::DragFloat("y##rot", &rotations.y, 0.5f);
			ImGui::DragFloat("z##rot", &rotations.z, 0.5f);
			float& scale = m_model->getScaleRef();
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
			m_model->updateModelMatrix();
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
				m_model->getLoadableDataRef().objData = objData;
				DrawableData::Default *defaultData = new DrawableData::Default(objData);
				if (m_model->getDrawableRef().drawableData != nullptr)
					delete m_model->getDrawableRef().drawableData;
				m_model->getDrawableRef().drawableData = defaultData;
				RenderPipeline::Default* defaultPipeline = new RenderPipeline::Default();
				if (m_model->getDrawableRef().renderPipeline != nullptr)
					delete m_model->getDrawableRef().renderPipeline;
				m_model->getDrawableRef().renderPipeline = defaultPipeline;
			}
		}
		// close
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::End();
}

void UI::ObjectMenu::setDataPtrs(
	bool* isOpen,
	Model* model)
{
	m_isOpen = isOpen;
	m_model = model;
}

bool UI::ObjectMenu::isDataPtrsSetup() const
{
	return (m_isOpen != nullptr)
	       && (m_model != nullptr);
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

std::string UI::ObjectMenu::getNextUniqueName()
{
	static int index = 0;
	return "Object " + std::to_string(index++);
}


