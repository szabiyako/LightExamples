#pragma once

#include "RenderingType.h"
#include <vector>
#include "LoadableData/Image/Image.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"

namespace UI {
struct RenderingMenuDataRef
{
	RenderingType &renderingType;
	int &nRaysMax;
	std::vector<Image> &skyboxImages;
	Texture::CubeMap &skyboxCubeMap;
	bool &enableVSync;
	bool &enableSSAO;
	
	RenderingMenuDataRef(
		RenderingType &renderingType,
		int &nRaysMax,
		std::vector<Image> &skyboxImages,
		Texture::CubeMap &skyboxCubeMap,
		bool &enableVSync,
		bool &enableSSAO);
};
}
