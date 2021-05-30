#include "ObjectsMenuDataRef.h"

UI::ObjectsMenuDataRef::ObjectsMenuDataRef(
	std::vector<LoadableData>& loadableDataVector,
	std::vector<Drawable>& drawableVector,
	std::vector<glm::mat4x4>& modelMatrixVector)
	: loadableDataVector(loadableDataVector),
	  drawableVector(drawableVector),
	  modelMatrixVector(modelMatrixVector)
{
}
