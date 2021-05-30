#pragma once

#include "UI/Element.h"

#include <vector>
#include "LoadableData/LoadableData.h"
#include "Drawable/Drawable.h"

namespace UI {
class ObjectMenu : public Element
{
public:
	ObjectMenu(const ImVec2 &windowPos = ImVec2(645, 40));
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
	void setDataPtrs(
		bool *isOpen,
		LoadableData* loadableData,
		Drawable* drawable,
		glm::mat4x4 *modelMatrix);
	bool isDataPtrsSetup() const;

	std::string getUniqueName() const;
	std::string getName() const;
	ImVec2 getWindowPos() const;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	std::string m_uniqueName;
	std::string m_name;
	std::string m_newName;
	std::string m_fileName = "File";

	bool *m_isOpen = nullptr;

	LoadableData *m_loadableData = nullptr;
	Drawable *m_drawable = nullptr;
	glm::mat4x4 *m_modelMatrix = nullptr;

	ImVec2 m_windowPos;

	bool m_isVisible = true;
	bool m_cullFaces = true;
	float m_color[3] = { 1.f, 1.f, 1.f };

	glm::vec3 m_position = { 0.f, 0.f, 0.f };
	glm::vec3 m_rotation = { 0.f, 0.f, 0.f };
	float m_scale = 1.f;
};
}

