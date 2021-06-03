#include "LightSrc.h"

#include "LoadableData/ObjData/ObjData.h"
#include "LoadableData/ObjData/Tools/Import.h"

#include "Utils/Console.h"

//Render types
#include "Drawable/DrawableData/Default/Default.h"
#include "Drawable/RenderPipeline/Default/Default.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Model/Model.h"

LightSrc::LightSrc()
{
	setType(Type::DIRECTIONAL);
}

LightSrc::~LightSrc()
{
}

void LightSrc::deleteDrawable()
{
	if (m_drawable.drawableData != nullptr)
		delete m_drawable.drawableData;
	if (m_drawable.renderPipeline != nullptr)
		delete m_drawable.renderPipeline;
}

Drawable LightSrc::getDrawable() const
{
	return m_drawable;
}

void LightSrc::setType(const Type& type)
{
	if (m_type == type)
		return;

	m_type = type;

	ObjData objData;
	std::string errorMessage;
	if (m_type == Type::DIRECTIONAL)
		ObjDataTools::Import::fromFile(objData, errorMessage, "res/models/arrow.obj");
	else if (m_type == Type::POINT)
		ObjDataTools::Import::fromFile(objData, errorMessage, "res/models/sphere.obj");
	else if (m_type == Type::SPOTLIGHT)
		ObjDataTools::Import::fromFile(objData, errorMessage, "res/models/projector.obj");
	const bool isLoaded = errorMessage.empty();
	if (!isLoaded) {
		Console::print("Failed to load model in lightSrc " + errorMessage + '\n');
	}
	DrawableData::Default* defaultData = new DrawableData::Default(objData);
	if (m_drawable.drawableData != nullptr)
		delete m_drawable.drawableData;
	m_drawable.drawableData = defaultData;
	if (m_drawable.renderPipeline == nullptr) {
		RenderPipeline::Default* defaultPipeline = new RenderPipeline::Default();
		m_drawable.renderPipeline = defaultPipeline;
	}
}

void LightSrc::setEnabled(const bool value)
{
	m_isEnabled = value;
}

void LightSrc::setShadowsEnabled(const bool value)
{
	m_isShadowsEnabled = value;
}

LightSrc::Type LightSrc::getType() const
{
	return m_type;
}

bool LightSrc::isEnabled() const
{
	return m_isEnabled;
}

bool LightSrc::isShadowsEnabled() const
{
	return m_isShadowsEnabled;
}

glm::mat4x4 LightSrc::getModelMatrix() const
{
	return m_modelMatrix;
}

bool& LightSrc::getEnabledRef()
{
	return m_isEnabled;
}

bool& LightSrc::getShadowsEnabledRef()
{
	return m_isShadowsEnabled;
}

glm::vec3& LightSrc::getPosRef()
{
	return m_position;
}

glm::vec3& LightSrc::getRotationRef()
{
	return m_rotation;
}

float& LightSrc::getScaleRef()
{
	return m_scale;
}

void LightSrc::updateModelMatrix()
{
	glm::mat4x4 modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4x4 undoRotated = glm::inverse(modelMatrix);
	m_modelMatrix = glm::translate(modelMatrix, glm::vec3(undoRotated * glm::vec4(m_position, 1.f)));
}

void LightSrc::updateDirection()
{
	glm::mat4x4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	m_dir = modelMatrix * glm::vec4(m_dir, 0.f);
}

glm::vec3 LightSrc::getPos() const
{
	return m_position;
}

glm::vec3 LightSrc::getDir() const
{
	return m_dir;
}

glm::vec3 LightSrc::getLightColor() const
{
	return m_lightColor;
}

void LightSrc::setLightColor(const glm::vec3& color)
{
	m_lightColor = color;
}

glm::vec3 LightSrc::getModelColor() const
{
	return m_modelColor;
}

void LightSrc::setModelColor(const glm::vec3& color)
{
	if (m_modelColor == color)
		return;
	m_modelColor = color;
	RenderPipeline::Default* defaultPipeline = dynamic_cast<RenderPipeline::Default*>(m_drawable.renderPipeline);
	ASSERT(defaultPipeline != nullptr); // RenderPipeline is not Default type
	defaultPipeline->setColor(m_modelColor);
}

bool LightSrc::isVisible() const
{
	return m_drawable.isVisible();
}

void LightSrc::setVisible(const bool value)
{
	m_drawable.setVisible(value);
}
