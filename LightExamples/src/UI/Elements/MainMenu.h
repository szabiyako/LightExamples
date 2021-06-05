#pragma once

#include "UI/Element.h"
#include "UI/DataRef.h"

#include "Menus/DebugMenu.h"
#include "Menus/CameraMenu.h"
#include "Menus/RenderingMenu.h"
#include "Menus/ObjectsMenu.h"
#include "Menus/LightsMenu.h"

class Camera;

namespace UI {
class MainMenu : public Element
{
public:
	MainMenu(const DataRef &dataRef);
	void process(const int windowWidth, const int windowHeight, bool& enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	bool m_isDebugMenuOpened = false;
	DebugMenu m_debugMenu;

	bool m_isCameraMenuOpened = false;
	CameraMenu m_cameraMenu;

	bool m_isRenderingMenuOpened = false;
	RenderingMenu m_renderingMenu;

	bool m_isObjectsMenuOpened = false;
	ObjectsMenu m_objectsMenu;

	bool m_isLightsMenuOpened = false;
	LightsMenu m_lightsMenu;

	//Temp
	bool m_isDemoWindowOpened = false;
};
}

