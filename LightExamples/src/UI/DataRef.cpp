#include "DataRef.h"

UI::DataRef::DataRef(
	    bool &enableKeysInput,
		const DebugMenuDataRef &debugMenuDataRef,
		const CameraMenuDataRef &cameraMenuDataRef,
		const RenderingMenuDataRef &renderingMenuDataRef,
		const ObjectsMenuDataRef &objectsMenuDataRef,
		const LightsMenuDataRef &lightsMenuDataRef)
	: enableKeysInput(enableKeysInput),
	  debugMenuDataRef(debugMenuDataRef),
	  cameraMenuDataRef(cameraMenuDataRef),
	  renderingMenuDataRef(renderingMenuDataRef),
	  objectsMenuDataRef(objectsMenuDataRef),
	  lightsMenuDataRef(lightsMenuDataRef)
{
}
