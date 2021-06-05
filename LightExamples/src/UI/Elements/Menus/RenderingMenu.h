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
	void backgroundHeader();
	void loadCubeMapTexturefileDialog(const size_t textureIndex);
	void applyCubeMap();
	static std::string getRenderTypeAsString(const RenderingType &renderingType);
	ImGuiWindowFlags m_windowFlags = 0;

	bool &m_isOpen;
	RenderingMenuDataRef m_dataRef;

	bool m_useCubeMap = false;
	float m_cubeMapColor[3] = { 0.1f, 0.1f, 0.1f };
	std::vector<std::string> m_cubeMapImageFileNames;
};
}

