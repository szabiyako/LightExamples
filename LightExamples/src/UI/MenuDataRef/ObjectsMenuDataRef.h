#pragma once

#include "LoadableData/LoadableData.h"
#include "Drawable/Drawable.h"
#include "glm/glm.hpp"
#include <vector>

namespace UI {
struct ObjectsMenuDataRef
{
	std::vector<LoadableData> &loadableDataVector;
	std::vector<Drawable> &drawableVector;
	std::vector<glm::mat4x4> &modelMatrixVector;

	ObjectsMenuDataRef(
		std::vector<LoadableData> &loadableDataVector,
		std::vector<Drawable> &drawableVector,
		std::vector<glm::mat4x4> &modelMatrixVector);
};
}