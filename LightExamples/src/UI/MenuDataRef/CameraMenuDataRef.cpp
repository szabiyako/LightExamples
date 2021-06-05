#include "CameraMenuDataRef.h"

UI::CameraMenuDataRef::CameraMenuDataRef(
	Camera &camera,
	float &cameraSpeed,
	float &cameraSensitivity)
	: camera(camera),
	  cameraSpeed(cameraSpeed),
	  cameraSensitivity(cameraSensitivity)
{
}
