#include "RenderingMenuDataRef.h"

UI::RenderingMenuDataRef::RenderingMenuDataRef(
	RenderingType& renderingType,
	std::vector<Image> &skyboxImages,
	Texture::CubeMap &skyboxCubeMap,
	unsigned int& maxFPS,
	bool& enableVSync)
	: renderingType(renderingType),
	  skyboxImages(skyboxImages),
	  skyboxCubeMap(skyboxCubeMap),
	  maxFPS(maxFPS),
	  enableVSync(enableVSync)
{
}
