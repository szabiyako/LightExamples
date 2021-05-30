#include "Drawable.h"

Drawable::Drawable(
	DrawableData::DrawableData *drawableData,
	RenderPipeline::RenderPipeline *renderPipeline)
	: drawableData(drawableData),
	  renderPipeline(renderPipeline)
{
}

void Drawable::show()
{
	m_visible = true;
}

void Drawable::hide()
{
	m_visible = false;
}

void Drawable::setVisible(const bool visible)
{
	m_visible = visible;
}

bool Drawable::isVisible() const
{
	return m_visible;
}

bool Drawable::canBeDrawn() const
{
	return (drawableData != nullptr) && (renderPipeline != nullptr);
}
