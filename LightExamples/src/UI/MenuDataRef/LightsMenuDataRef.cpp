#include "LightsMenuDataRef.h"

UI::LightsMenuDataRef::LightsMenuDataRef(
	std::vector<LightSrc> &lightSources,
	bool &resetFrames)
	: lightSources(lightSources),
	  resetFrames(resetFrames)
{
}