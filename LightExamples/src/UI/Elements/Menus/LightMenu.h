#pragma once

#include "UI/Element.h"

#include <vector>
#include "LightSrc/LightSrc.h"

namespace UI {
class LightMenu : public Element
{
public:
	LightMenu(const ImVec2 &windowPos = ImVec2(645, 445));
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
	void setDataPtrs(
		bool *isOpen,
		LightSrc* lightSrc,
		bool *resetFrames);
	bool isDataPtrsSetup() const;

	std::string getUniqueName() const;
	std::string getName() const;
	ImVec2 getWindowPos() const;
private:
	static std::string getNextUniqueName();
	static std::string getTypeAsString(const LightSrc::Type &type);
	static std::vector<LightSrc::Type> getTypesVector();
	ImGuiWindowFlags m_windowFlags = 0;

	std::string m_uniqueName;
	std::string m_name;
	std::string m_newName;

	bool *m_isOpen = nullptr;
	ImVec2 m_windowPos;

	LightSrc *m_lightSrc = nullptr;
	bool *m_resetFrames = nullptr;
};
}