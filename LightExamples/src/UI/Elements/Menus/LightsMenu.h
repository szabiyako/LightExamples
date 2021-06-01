#pragma once

#include "UI/Element.h"
#include "UI/MenuDataRef/LightsMenuDataRef.h"

#include "LightMenu.h"

namespace UI {
class LightsMenu : public Element
{
public:
	LightsMenu(
		bool &isOpen,
		const LightsMenuDataRef& dataRef);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	bool& m_isOpen;
	LightsMenuDataRef m_dataRef;

	std::vector<LightMenu> m_lightMenus;
	std::vector<bool> m_showObjectMenu;
};
}