#pragma once

#include "UI/Element.h"

#include "RayTracing/RayTracer.h"
#include "RenderingType.h"

namespace UI {

class RayTracingProgressBar : public Element
{
public:
	RayTracingProgressBar(RayTracer &rayTracer, RenderingType &renderingType);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	RayTracer &m_rayTracerRef;
	RenderingType &m_renderingTypeRef;
};

}