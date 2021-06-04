#pragma once

#include "RenderingType.h"
#include <vector>
#include "LoadableData/Image/Image.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"

namespace UI {
struct RenderingMenuDataRef
{
	RenderingType &renderingType;
	std::vector<Image> &skyboxImages;
	Texture::CubeMap &skyboxCubeMap;
	unsigned int &maxFPS;
	bool &enableVSync;
	
	RenderingMenuDataRef(
		RenderingType &renderingType,
		std::vector<Image> &skyboxImages,
		Texture::CubeMap &skyboxCubeMap,
		unsigned int &maxFPS,
		bool &enableVSync);
};
}
