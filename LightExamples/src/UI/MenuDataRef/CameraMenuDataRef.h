#pragma once

#include "Camera.h"

namespace UI {
struct CameraMenuDataRef
{
	Camera &camera;
	float &cameraSpeed;
	float &cameraSensitivity;

	CameraMenuDataRef(
		Camera &camera,
		float &cameraSpeed,
		float &cameraSensitivity);
};
}

