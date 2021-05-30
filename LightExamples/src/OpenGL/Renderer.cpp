#include "Renderer.h"

#include <GL/glew.h>
#include "ErrorHandler.h"

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(
			const Drawable& drawable,
			const glm::mat4x4& modelMatrix,
			const glm::mat4x4& viewMatrix,
			const glm::mat4x4& projectionMatrix)
{
	if (drawable.canBeDrawn() && drawable.isVisible())
		draw(*drawable.drawableData,
			 *drawable.renderPipeline,
			 modelMatrix,
			 viewMatrix,
			 projectionMatrix);
}

void Renderer::draw(
			const DrawableData::DrawableData& drawableData,
			RenderPipeline::RenderPipeline& renderPipeline,
			const glm::mat4x4& modelMatrix,
			const glm::mat4x4& viewMatrix,
			const glm::mat4x4& projectionMatrix)
{
	renderPipeline.draw(
				drawableData,
				modelMatrix,
				viewMatrix,
				projectionMatrix);
}

void Renderer::draw(UI::UI *ui)
{
	ui->draw();
}

void Renderer::draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader)
{
	shader.bind();
	va.bind();
	ib.bind();
	
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader, const Texture & texture)
{
	shader.bind();
	va.bind();
	ib.bind();
	texture.bind();
	
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawInstanced(const VertexArray & va, const IndexBuffer & ib, const unsigned int & count, const Shader & shader, const Texture & texture)
{
	shader.bind();
	va.bind();
	ib.bind();
	texture.bind();

	GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}
