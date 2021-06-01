#pragma once

#include "LoadableData/LoadableData.h"
#include "Drawable/Drawable.h"
#include "glm/glm.hpp"
#include <vector>

namespace UI {
struct LightsMenuDataRef
{
	std::vector<LoadableData> &loadableDataVector;
	std::vector<Drawable> &drawableVector;
	std::vector<glm::mat4x4> &modelMatrixVector;

	LightsMenuDataRef(
		std::vector<LoadableData> &loadableDataVector,
		std::vector<Drawable> &drawableVector,
		std::vector<glm::mat4x4> &modelMatrixVector);
};
}
