#pragma once

#include "MenuDataRef/DebugMenuDataRef.h"
#include "MenuDataRef/CameraMenuDataRef.h"
#include "MenuDataRef/RenderingMenuDataRef.h"
#include "MenuDataRef/ObjectsMenuDataRef.h"
#include "MenuDataRef/LightsMenuDataRef.h"

namespace UI {
struct DataRef
{
	bool &enableKeysInput;
	
	DebugMenuDataRef debugMenuDataRef;
	CameraMenuDataRef cameraMenuDataRef;
	RenderingMenuDataRef renderingMenuDataRef;
	ObjectsMenuDataRef objectsMenuDataRef;
	LightsMenuDataRef lightsMenuDataRef;

	DataRef(
		bool &enableKeysInput,
		const DebugMenuDataRef &debugMenuDataRef,
		const CameraMenuDataRef &cameraMenuDataRef,
		const RenderingMenuDataRef &renderingMenuDataRef,
		const ObjectsMenuDataRef &objectsMenuDataRef,
		const LightsMenuDataRef &lightsMenuDataRef);
};
}

