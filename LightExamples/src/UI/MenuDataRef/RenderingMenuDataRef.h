#pragma once

#include "RenderingType.h"

namespace UI {
struct RenderingMenuDataRef
{
	RenderingType &renderingType;
	unsigned int &maxFPS;
	bool &enableVSync;
	
	RenderingMenuDataRef(
		RenderingType &renderingType,
		unsigned int &maxFPS,
		bool &enableVSync);
};
}
