#pragma once

#include "UI/Element.h"
#include "UI/MenuDataRef/RenderingMenuDataRef.h"

//TMP
#include <string>

namespace UI {
class RenderingMenu : public Element
{
public:
	RenderingMenu(bool& isOpen, const RenderingMenuDataRef &dataRef);
	void process(const int windowWidth, const int windowHeight, bool& enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	bool &m_isOpen;
	RenderingMenuDataRef m_dataRef;

	//TMP
	std::string renderType = "Default";
};
}

