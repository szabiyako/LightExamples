#pragma once

#include "LightSrc/LightSrc.h"
#include "glm/glm.hpp"
#include <vector>

namespace UI {
struct LightsMenuDataRef
{
	std::vector<LightSrc> &lightSources;
	bool &resetFrames;

	LightsMenuDataRef(
		std::vector<LightSrc> &lightSources,
		bool &resetFrames);
};
}
