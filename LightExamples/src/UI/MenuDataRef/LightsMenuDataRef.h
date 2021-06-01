#pragma once

#include "LightSrc/LightSrc.h"
#include "glm/glm.hpp"
#include <vector>

namespace UI {
struct LightsMenuDataRef
{
	std::vector<LightSrc> &lightSources;

	LightsMenuDataRef(
		std::vector<LightSrc> &lightSources);
};
}
