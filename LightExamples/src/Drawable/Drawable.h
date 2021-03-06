#pragma once

#include "DrawableData/DrawableData.h"
#include "RenderPipeline/RenderPipeline.h"

//#include "Utils/NonCopyable.h"// : public NonCopyable

// Needs to delete variable ptrs before deleting Drawable
struct Drawable
{
	DrawableData::DrawableData *drawableData = nullptr;
	RenderPipeline::RenderPipeline *renderPipeline = nullptr;

	Drawable(DrawableData::DrawableData *drawableData = nullptr,
		     RenderPipeline::RenderPipeline *renderPipeline = nullptr);

	void show();
	void hide();
	void setVisible(const bool visible);
	bool isVisible() const;
	bool canBeDrawn() const;
private:
	bool m_visible = true;
};

