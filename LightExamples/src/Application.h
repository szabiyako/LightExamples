#pragma once

#include "OpenGL/OpenGLinit.h"

#include <iostream>
#include <fstream>
#include <string>

//for GLCall

#include "opengl/Renderer.h"
#include "opengl/VertexBuffer.h"
#include "opengl/VertexBufferLayout.h"
#include "opengl/IndexBuffer.h"
#include "opengl/VertexArray.h"
#include "opengl/Shader.h"
#include "Camera.h"

#include "glm/glm.hpp"

//rotation
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//random
#include <ctime>

#include <UI/UI.h>

#include "LoadableData/LoadableData.h"

#include "RenderingType.h"

#include "Model/Model.h"
#include "LightSrc/LightSrc.h"

class Application
{
	GLFWwindow *m_window = nullptr;

	UI::UI *m_ui = nullptr;

	float m_deltaTime;
	float m_lastFrameTime;

	bool m_enableKeysInput = true;

	// DebugMenuData
	bool m_enableFPScounter = true;
	bool m_enableCursor = true;

	// CameraMenuData
	Camera m_camera;
	float m_cameraSpeed = 40.f;

	// RenderingMenuData
	RenderingType m_renderingType = RenderingType::DEFAULT;
	std::vector<Image> m_skyboxImages;
	Texture::CubeMap *m_skyboxCubeMap = nullptr;
	unsigned int m_maxFPS = 0;
	bool m_enableVSync = false;

	bool m_lastEnableVSync = false;

	// ObjectsMenuData
	std::vector<Model> m_models;

	// LightsMenuData
	std::vector<LightSrc> m_lightSources;


	bool showCursor = false;

	//keys
	bool fullsize = false;
	bool fullscreen = false;
	bool wireframerender = false;


	void updateDeltaTime();

	//Callback functions
	void window_size_callback(GLFWwindow* window, int width, int height);
	void cursor_callback(GLFWwindow* window, double xpos, double ypos);

	//SinglePress
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, int button, int action, int mods);

	void setCallbackFunctions(GLFWwindow * window);

	//HoldPress
	void keysinput(GLFWwindow* window);
public:
	Application();
	~Application();
	void run();
};

