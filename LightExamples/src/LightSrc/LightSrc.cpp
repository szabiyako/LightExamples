#include "LightSrc.h"

#include "LoadableData/ObjData/ObjData.h"
#include "LoadableData/ObjData/Tools/Import.h"

#include "Utils/Console.h"

#include "Drawable/DrawableData/Default/Default.h"
#include "Drawable/RenderPipeline/Default/Default.h"

LightSrc::LightSrc()
{
	ObjData objData;
	std::string errorMessage;
	ObjDataTools::Import::fromFile(objData, errorMessage, "D:/Git/QtProjects/cube.obj");
	const bool isLoaded = errorMessage.empty();
	if (!isLoaded) {
		Console::print("Failed to load \"Cube\" in lightSrc " + errorMessage + '\n');
	}
	else {
		Console::print("Loaded \"Cube\" in lightSrc\n");
	}
	DrawableData::Default* defaultData = new DrawableData::Default(objData);
	m_drawable.drawableData = defaultData;
	RenderPipeline::Default* defaultPipeline = new RenderPipeline::Default();
	m_drawable.renderPipeline = defaultPipeline;
}

LightSrc::~LightSrc()
{
	if (m_drawable.drawableData != nullptr)
		delete m_drawable.drawableData;
	if (m_drawable.renderPipeline != nullptr)
		delete m_drawable.renderPipeline;
}

void LightSrc::setType(const Type& type)
{
	m_type = type;
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
