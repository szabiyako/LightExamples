#pragma once

#include "RenderingType.h"
#include <vector>
#include "LoadableData/Image/Image.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"
#include "RayTracing/RayTracer.h"

namespace UI {
struct RenderingMenuDataRef
{
	RenderingType &renderingType;
	RayTracer &rayTracer;
	std::vector<Image> &skyboxImages;
	Texture::CubeMap &skyboxCubeMap;
	bool &enableVSync;
	bool &enableSSAO;
	
	RenderingMenuDataRef(
		RenderingType &renderingType,
		RayTracer &rayTracer,
		std::vector<Image> &skyboxImages,
		Texture::CubeMap &skyboxCubeMap,
		bool &enableVSync,
		bool &enableSSAO);
};
}
