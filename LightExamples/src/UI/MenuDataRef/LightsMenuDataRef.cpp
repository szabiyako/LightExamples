#include "LightsMenuDataRef.h"

UI::LightsMenuDataRef::LightsMenuDataRef(
	std::vector<LightSrc> &lightSources,
	std::vector<Model> &models)
	: lightSources(lightSources),
	  models(models)
{
}