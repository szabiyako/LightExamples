#include "ObjectsMenuDataRef.h"

UI::ObjectsMenuDataRef::ObjectsMenuDataRef(
	std::vector<Model> &models,
	std::vector<LightSrc> &lightSources)
	: models(models),
	  lightSources(lightSources)
{
}
