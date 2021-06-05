#include "RenderingMenuDataRef.h"

UI::RenderingMenuDataRef::RenderingMenuDataRef(
	RenderingType& renderingType,
	std::vector<Image> &skyboxImages,
	Texture::CubeMap &skyboxCubeMap,
	bool& enableVSync)
	: renderingType(renderingType),
	  skyboxImages(skyboxImages),
	  skyboxCubeMap(skyboxCubeMap),
	  enableVSync(enableVSync)
{
}
