#include "LightsMenuDataRef.h"

UI::LightsMenuDataRef::LightsMenuDataRef(
	std::vector<LoadableData>& loadableDataVector,
	std::vector<Drawable>& drawableVector,
	std::vector<glm::mat4x4>& modelMatrixVector)
	: loadableDataVector(loadableDataVector),
	drawableVector(drawableVector),
	modelMatrixVector(modelMatrixVector)
{
}