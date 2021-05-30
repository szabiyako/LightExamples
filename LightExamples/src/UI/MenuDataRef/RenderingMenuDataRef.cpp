#include "RenderingMenuDataRef.h"

UI::RenderingMenuDataRef::RenderingMenuDataRef(
	RenderingType& renderingType,
	unsigned int& maxFPS,
	bool& enableVSync)
	: renderingType(renderingType),
	  maxFPS(maxFPS),
	  enableVSync(enableVSync)
{
}
