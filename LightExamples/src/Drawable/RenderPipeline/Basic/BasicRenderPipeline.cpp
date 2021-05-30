#include "BasicRenderPipeline.h"

#include "GL/glew.h"
#include "OpenGL/ErrorHandler.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Drawable/DrawableData/Geometry/Geometry.h"
#include <iostream>

using namespace RenderPipeline;

BasicRenderPipeline::BasicRenderPipeline()
	: m_shader("res/shaders/Basic.shader")
{
}

void BasicRenderPipeline::draw(
				const DrawableData::DrawableData &drawableData,
				const glm::mat4x4 &modelMatrix,
				const glm::mat4x4 &viewMatrix,
				const glm::mat4x4 &projectionMatrix)
{
	DrawableData::DrawableData& drawData = const_cast<DrawableData::DrawableData&>(drawableData);
	DrawableData::Geometry* geometry = dynamic_cast<DrawableData::Geometry*>(&drawData);
	if (geometry == nullptr) {
		std::cout << "DrawableData is not a Geometry!";
		return;
	}

	m_shader.bind();
	glm::mat4x4 normalModelMatrix = modelMatrix;
	normalModelMatrix[3][0] = 0;
	normalModelMatrix[3][1] = 0;
	normalModelMatrix[3][2] = 0;
	m_shader.setUniformMatrix4f("u_modelMatrix", modelMatrix);
	m_shader.setUniformMatrix4f("u_viewMatrix", viewMatrix);
	m_shader.setUniformMatrix4f("u_projectionMatrix", projectionMatrix);
	glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(viewMatrix));
	m_shader.setUniformMatrix4f("u_normalMatrix", normalMatrix);
	m_shader.setUniformMatrix4f("u_normalModelMatrix", normalModelMatrix);
	m_shader.setUniformVec3f("u_color", m_color);
	
	geometry->m_vertexArray.bind();
	geometry->m_indexBuffer.bind();
	if (m_cullFace)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	GLCall(glDrawElements(GL_TRIANGLES, geometry->m_indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

void BasicRenderPipeline::setCullFace(const bool value)
{
	m_cullFace = value;
}

void BasicRenderPipeline::setColor(const glm::vec3 &color)
{
	m_color = glm::clamp(color, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
}
