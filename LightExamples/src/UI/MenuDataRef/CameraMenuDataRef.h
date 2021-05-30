#pragma once

#include "Camera.h"

namespace UI {
struct CameraMenuDataRef
{
	Camera &camera;
	float &cameraSpeed;

	CameraMenuDataRef(
		Camera &camera,
		float &cameraSpeed);
};
}

