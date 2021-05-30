#pragma once

#include "UI/Element.h"
#include "UI/MenuDataRef/ObjectsMenuDataRef.h"

#include "ObjectMenu.h"

namespace UI {
class ObjectsMenu : public Element
{
public:
	ObjectsMenu(
		bool &isOpen,
		const ObjectsMenuDataRef &dataRef);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	bool& m_isOpen;
	ObjectsMenuDataRef m_dataRef;

	std::vector<ObjectMenu> m_objectMenus;
	std::vector<bool> m_showObjectMenu;
};
}

