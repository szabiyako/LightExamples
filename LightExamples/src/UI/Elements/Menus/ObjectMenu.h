#pragma once

#include "UI/Element.h"

#include <vector>
#include "Model/Model.h"

namespace UI {
class ObjectMenu : public Element
{
public:
	ObjectMenu(const ImVec2 &windowPos = ImVec2(645, 40));
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
	void setDataPtrs(
		bool *isOpen,
		Model* model);
	bool isDataPtrsSetup() const;

	std::string getUniqueName() const;
	std::string getName() const;
	ImVec2 getWindowPos() const;
private:
	static std::string getNextUniqueName();

	ImGuiWindowFlags m_windowFlags = 0;

	std::string m_uniqueName;
	std::string m_name;
	std::string m_newName;
	std::string m_fileName = "File";

	ImVec2 m_windowPos;
	
	bool *m_isOpen = nullptr;

	Model *m_model = nullptr;
};
}

