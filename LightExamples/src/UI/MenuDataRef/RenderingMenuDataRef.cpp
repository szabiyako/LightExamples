#include "RenderingMenuDataRef.h"

UI::RenderingMenuDataRef::RenderingMenuDataRef(
	RenderingType& renderingType,
	RayTracer &rayTracer,
	std::vector<Image> &skyboxImages,
	Texture::CubeMap &skyboxCubeMap,
	bool& enableVSync,
	bool &enableSSAO)
	: renderingType(renderingType),
	  rayTracer(rayTracer),
	  skyboxImages(skyboxImages),
	  skyboxCubeMap(skyboxCubeMap),
	  enableVSync(enableVSync),
	  enableSSAO(enableSSAO)
{
}
