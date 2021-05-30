#pragma once

#include "UI/Element.h"
#include "UI/MenuDataRef/DebugMenuDataRef.h"

#include "UI/Elements/Utils/Console.h"

namespace UI {
class DebugMenu : public Element
{
public:
	DebugMenu(
		bool& isOpen,
		const DebugMenuDataRef &dataRef);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	bool& m_isOpen;
	DebugMenuDataRef m_dataRef;

	bool m_showConsole = false;
	Console m_console;
	bool m_showUiDebug = false;
};
}
