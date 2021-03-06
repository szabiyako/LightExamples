#pragma once

#include "Drawable/RenderPipeline/RenderPipeline.h"

#include "OpenGL/Shader.h"

namespace RenderPipeline {
class Default : public RenderPipeline
{
public:
	Default();
	void draw(
		const DrawableData::DrawableData& drawableData,
		const glm::mat4x4& modelMatrix,
		const glm::mat4x4& viewMatrix,
		const glm::mat4x4& projectionMatrix) override;
	void setCullFace(const bool value);
	void setColor(const glm::vec3& color);
	bool getCullFace() const;
	glm::vec3 getColor() const;
private:
	Shader m_shader;
	bool m_cullFace = true;
	glm::vec3 m_color = glm::vec3(1.f, 1.f, 1.f);
};
}

