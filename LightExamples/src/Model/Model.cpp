#include "Model.h"

#include "LoadableData/ObjData/ObjData.h"
#include "LoadableData/ObjData/Tools/Import.h"

#include "Utils/Console.h"

//Render types
#include "Drawable/DrawableData/Default/Default.h"
#include "Drawable/RenderPipeline/Default/Default.h"

#include "glm/gtc/matrix_transform.hpp"

Model::Model(const std::vector<LightSrc>& lightSources)
	: m_lightSources(lightSources)
{
}

Model::~Model()
{
}

void Model::deleteDrawable()
{
	if (m_drawable.drawableData != nullptr)
		delete m_drawable.drawableData;
	if (m_drawable.renderPipeline != nullptr)
		delete m_drawable.renderPipeline;
}

Drawable Model::getDrawable() const
{
	return m_drawable;
}

void Model::setRenderType(const RenderType& type)
{
	m_renderType = type;
}

Model::RenderType Model::getRenderType() const
{
	return m_renderType;
}

glm::mat4x4 Model::getModelMatrix() const
{
	return m_modelMatrix;
}

glm::vec3& Model::getPosRef()
{
	return m_position;
}

glm::vec3& Model::getRotationRef()
{
	return m_rotation;
}

float& Model::getScaleRef()
{
	return m_scale;
}

void Model::updateModelMatrix()
{
	glm::mat4x4 modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4x4 undoRotated = glm::inverse(modelMatrix);
	m_modelMatrix = glm::translate(modelMatrix, glm::vec3(undoRotated * glm::vec4(m_position, 1.f)));
}

glm::vec3 Model::getColor() const
{
	return m_color;
}

void Model::setColor(const glm::vec3& color)
{
	if (m_color == color)
		return;
	m_color = color;
	RenderPipeline::Default* defaultPipeline = dynamic_cast<RenderPipeline::Default*>(m_drawable.renderPipeline);
	ASSERT(defaultPipeline != nullptr); // RenderPipeline is not Default type
	defaultPipeline->setColor(m_color);
}
