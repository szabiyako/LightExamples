#pragma once

#include "Model/Model.h"
#include "LightSrc/LightSrc.h"
#include <vector>

namespace UI {
struct ObjectsMenuDataRef
{
	std::vector<Model> &models;
	std::vector<LightSrc> &lightSources;

	ObjectsMenuDataRef(
		std::vector<Model> &models,
		std::vector<LightSrc> &lightSources);
};
}