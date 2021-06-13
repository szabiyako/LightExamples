#include "RenderingMenuDataRef.h"

UI::RenderingMenuDataRef::RenderingMenuDataRef(
	RenderingType& renderingType,
	int& nRaysMax,
	std::vector<Image> &skyboxImages,
	Texture::CubeMap &skyboxCubeMap,
	bool& enableVSync,
	bool &enableSSAO)
	: renderingType(renderingType),
	  nRaysMax(nRaysMax),
	  skyboxImages(skyboxImages),
	  skyboxCubeMap(skyboxCubeMap),
	  enableVSync(enableVSync),
	  enableSSAO(enableSSAO)
{
}
