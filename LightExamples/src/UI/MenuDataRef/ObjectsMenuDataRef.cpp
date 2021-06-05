#include "ObjectsMenuDataRef.h"

UI::ObjectsMenuDataRef::ObjectsMenuDataRef(
	std::vector<Model> &models,
	std::vector<LightSrc> &lightSources,
	Texture::CubeMap &skyboxCubeMap)
	: models(models),
	  lightSources(lightSources),
	  skyboxCubeMap(skyboxCubeMap)
{
}
