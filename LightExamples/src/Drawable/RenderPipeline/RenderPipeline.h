#pragma once

#include "glm/glm.hpp"

#include "Drawable/DrawableData/DrawableData.h"
#include "Utils/NonCopyable.h"

namespace RenderPipeline {
class RenderPipeline : public NonCopyable
{
public:
	virtual void draw(
		const DrawableData::DrawableData& drawableData,
		const glm::mat4x4& modelMatrix,
		const glm::mat4x4& viewMatrix,
		const glm::mat4x4& projectionMatrix) = 0;
};
}
