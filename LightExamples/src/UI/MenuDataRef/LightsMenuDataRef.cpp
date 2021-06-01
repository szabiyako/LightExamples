#include "LightsMenuDataRef.h"

UI::LightsMenuDataRef::LightsMenuDataRef(
	std::vector<Drawable>& drawableVector,
	std::vector<glm::mat4x4>& modelMatrixVector)
	: drawableVector(drawableVector),
	  modelMatrixVector(modelMatrixVector)
{
}