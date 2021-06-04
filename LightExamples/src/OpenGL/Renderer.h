#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture/Texture.h"

#include "Drawable/Drawable.h"

#include "UI/UI.h"

class Renderer
{
private:

public:
	static void clear();

	static void draw(
		const Drawable &drawable,
		const glm::mat4x4& modelMatrix,
		const glm::mat4x4& viewMatrix,
		const glm::mat4x4& projectionMatrix);

	static void draw(
		const DrawableData::DrawableData &drawableData,
		RenderPipeline::RenderPipeline &renderPipeline,
		const glm::mat4x4 &modelMatrix,
		const glm::mat4x4 &viewMatrix,
		const glm::mat4x4 &projectionMatrix);

	static void draw(UI::UI *ui);

	//OLD STUFF
	static void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const Texture::Texture& texture);
	static void drawInstanced(const VertexArray& va, const IndexBuffer& ib, const unsigned int& count, const Shader& shader, const Texture::Texture& texture);
};


