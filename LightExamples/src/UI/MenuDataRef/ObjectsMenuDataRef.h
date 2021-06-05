#pragma once

#include "Model/Model.h"
#include "LightSrc/LightSrc.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"
#include <vector>

namespace UI {
struct ObjectsMenuDataRef
{
	std::vector<Model> &models;
	std::vector<LightSrc> &lightSources;
	Texture::CubeMap &skyboxCubeMap;

	ObjectsMenuDataRef(
		std::vector<Model> &models,
		std::vector<LightSrc> &lightSources,
		Texture::CubeMap &skyboxCubeMap);
};
}