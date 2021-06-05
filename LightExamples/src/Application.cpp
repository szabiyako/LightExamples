#include "Application.h"

#include "Utils/Console.h"

//Temp static---------------------------------------------------------------
#include <array>
std::array<float, 192> GetCubeVertex()
{
	return {
		// Position             UV             Normales
		-0.5f, 0.5f, 0.5f,   0.f, 1.f,     0.f,  0.f,  1.f,  //0
		 0.5f, 0.5f, 0.5f,   1.f, 1.f,     0.f,  0.f,  1.f,  //1   FRONT
		-0.5f,-0.5f, 0.5f,   0.f, 0.f,     0.f,  0.f,  1.f,  //2
		 0.5f,-0.5f, 0.5f,   1.f, 0.f,     0.f,  0.f,  1.f,  //3

		-0.5f, 0.5f,-0.5f,   0.f, 1.f,    -1.f,  0.f,  0.f,  //4
		-0.5f, 0.5f, 0.5f,   1.f, 1.f,    -1.f,  0.f,  0.f,  //5   LEFT
		-0.5f,-0.5f,-0.5f,   0.f, 0.f,    -1.f,  0.f,  0.f,  //6
		-0.5f,-0.5f, 0.5f,   1.f, 0.f,    -1.f,  0.f,  0.f,  //7

		-0.5f, 0.5f,-0.5f,   0.f, 1.f,     0.f,  1.f,  0.f,  //8
		 0.5f, 0.5f,-0.5f,   1.f, 1.f,     0.f,  1.f,  0.f,  //9   TOP
		-0.5f, 0.5f, 0.5f,   0.f, 0.f,     0.f,  1.f,  0.f,  //10
		 0.5f, 0.5f, 0.5f,   1.f, 0.f,     0.f,  1.f,  0.f,  //11

		-0.5f,-0.5f, 0.5f,   0.f, 1.f,     0.f, -1.f,  0.f,  //12
		 0.5f,-0.5f, 0.5f,   1.f, 1.f,     0.f, -1.f,  0.f,  //13  BOTTOM
		-0.5f,-0.5f,-0.5f,   0.f, 0.f,     0.f, -1.f,  0.f,  //14
		 0.5f,-0.5f,-0.5f,   1.f, 0.f,     0.f, -1.f,  0.f,  //15

		 0.5f, 0.5f, 0.5f,   0.f, 1.f,     1.f,  0.f,  0.f,  //16
		 0.5f, 0.5f,-0.5f,   1.f, 1.f,     1.f,  0.f,  0.f,  //17  RIGHT
		 0.5f,-0.5f, 0.5f,   0.f, 0.f,     1.f,  0.f,  0.f,  //18
		 0.5f,-0.5f,-0.5f,   1.f, 0.f,     1.f,  0.f,  0.f,  //19

		 0.5f, 0.5f,-0.5f,   0.f, 1.f,     0.f,  0.f, -1.f,  //20
		-0.5f, 0.5f,-0.5f,   1.f, 1.f,     0.f,  0.f, -1.f,  //21  BACK
		 0.5f,-0.5f,-0.5f,   0.f, 0.f,     0.f,  0.f, -1.f,  //22
		-0.5f,-0.5f,-0.5f,   1.f, 0.f,     0.f,  0.f, -1.f   //23
	};
}

// 0 - cube, 1 - skybox
std::array<unsigned int, 36> GetCubeIndex(int type = 0)
{
	if (type == 0)
		return {
			 0,  2,  1,//FRONT
			 1,  2,  3,

			 4,  6,  5,//LEFT
			 5,  6,  7,

			 8, 10,  9,//TOP
			 9, 10, 11,

			12, 14, 13,//BOTTOM
			13, 14, 15,

			16, 18, 17,//RIGHT
			17, 18, 19,

			20, 22, 21,//BACK
			21, 22, 23
		};
	return {
			0, 1, 2, //FRONT
			1, 3, 2,

			4, 5, 6, //LEFT
			5, 7, 6,

			8, 9, 10, //TOP
			9, 11, 10,

			12, 13, 14, //BOTTOM
			13, 15, 14,

			16, 17, 18, //RIGHT
			17, 19, 18,

			20, 21, 22, //BACK
			21, 23, 22
	};
}
//End temp static-------------------------------------------------

void Application::updateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrameTime;
	m_lastFrameTime = currentFrame;
}


void Application::window_size_callback(GLFWwindow * window, int width, int height)
{
	if (height == 0)
		return;
	m_camera.setAspectRatio((float)width / (float)height);
	m_ui->setWindowSize(width, height);
	glViewport(0, 0, width, height);
}

void Application::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (!m_enableKeysInput)
		return;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (showCursor)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			showCursor = false;
		}
		else
		{
			showCursor = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
	{
		if (wireframerender)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframerender = false;
		}
		else
		{
			wireframerender = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (fullsize)
		{
			glfwRestoreWindow(window);
			fullsize = false;
		}
		else
		{
			fullsize = true;
			glfwMaximizeWindow(window);
		}
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
		key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		if (fullscreen)
		{
			glfwSetWindowMonitor(window, NULL, 200, 200, 800, 600, GLFW_DONT_CARE);
			fullscreen = false;
			glfwRestoreWindow(window);
			fullsize = false;
		}
		else
		{
			int w, h;
			int x, y;
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);
			//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, x, y, w, h, GLFW_DONT_CARE);//mode->refreshRate);
			fullscreen = true;
		}
	}
}

void Application::cursor_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (!showCursor)
		m_camera.rotateFromCursor(xpos, ypos, 0.05f); //sensitivity = 0.05
	else
		m_camera.resetStartCursorPos(xpos, ypos);
}

void Application::mouse_callback(GLFWwindow * window, int button, int action, int mods)
{
	//if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
	//	...
	//}
}

void Application::setCallbackFunctions(GLFWwindow * window)
{
	Application* myPtr = this;
	glfwSetWindowUserPointer(window, myPtr);

	//Bind Resize func
	auto WindowSizeCallback = [](GLFWwindow* w, int a, int b)
	{
		static_cast<Application*>(glfwGetWindowUserPointer(w))->window_size_callback(w, a, b);
	};
	glfwSetWindowSizeCallback(window, WindowSizeCallback);

	//Bind Keys func
	auto KeyCallback = [](GLFWwindow* w, int a, int b, int c, int d)
	{
		static_cast<Application*>(glfwGetWindowUserPointer(w))->key_callback(w, a, b, c, d);
	};
	glfwSetKeyCallback(window, KeyCallback);

	//Bind Cursor func
	auto CursorCallback = [](GLFWwindow* w, double a, double b)
	{
		static_cast<Application*>(glfwGetWindowUserPointer(w))->cursor_callback(w, a, b);
	};
	glfwSetCursorPosCallback(window, CursorCallback);

	//Bind Mouse func
	auto MouseCallback = [](GLFWwindow* w, int a, int b, int c)
	{
		static_cast<Application*>(glfwGetWindowUserPointer(w))->mouse_callback(w, a, b, c);
	};
	glfwSetMouseButtonCallback(window, MouseCallback);
}


void Application::keysinput(GLFWwindow * window)
{
	if (!m_enableKeysInput)
		return;
	float distance = m_cameraSpeed * m_deltaTime; //(speed per frame) = (speed per second) * (time for frame)

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		distance *= 3.f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera.moveFrontToView(distance);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera.moveFrontToView(-distance);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera.moveRight(-distance);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera.moveRight(distance);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_camera.moveUp(distance);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)// && (int)cameraPos.y - 1 != 4 + (int)noise[1 + (int)cameraPos.z][(int)cameraPos.x])
		m_camera.moveUp(-distance);

	//GAMEPAD ps: why not?)
	GLFWgamepadstate state;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
	{
		if (std::abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]) > 0.2f)
			m_camera.moveRight(distance * state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);

		if (std::abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]) > 0.2f)
			m_camera.moveFrontToView(-distance * state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);

		if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
			m_camera.moveUp(distance);
		if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
			m_camera.moveUp(-distance);

		float xRotation = 0.f;
		if (std::abs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]) > 0.2f)
			xRotation = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];

		float yRotation = 0.f;
		if (std::abs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]) > 0.2f)
			yRotation = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

		m_camera.rotateFromJoystick(xRotation, yRotation, 0.05f); //sensitivity = 0.05
		//std::cout << "RIGHT_TRIGGER " << state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
	}
}

Application::Application()
	: m_window(nullptr),
	  m_deltaTime(0.f),
	  m_lastFrameTime(0.f),
	  m_camera(glm::vec3(0.f, 0.f, 7.5f)),
	  m_skyboxImages(std::vector<Image>(6))
{
	//Init srand
	std::srand(unsigned(std::time(0)));

	if (!OpenGLinit(m_window))
	{
		return;
	}

	//Bind callback functions
	setCallbackFunctions(m_window);

	//Set Mouse mode
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_skyboxCubeMap = new Texture::CubeMap();

	UI::DebugMenuDataRef debugMenuDataRef(m_enableFPScounter,m_enableCursor);
	UI::CameraMenuDataRef cameraMenuDataRef(m_camera, m_cameraSpeed);
	UI::RenderingMenuDataRef renderingMenuDataRef(m_renderingType, m_skyboxImages, *m_skyboxCubeMap, m_enableVSync);
	UI::ObjectsMenuDataRef objectsMenuDataRef(m_models, m_lightSources, *m_skyboxCubeMap);
	UI::LightsMenuDataRef lightsMenuDataRef(m_lightSources);
	UI::DataRef dataRef(
		m_enableKeysInput,
		debugMenuDataRef,
		cameraMenuDataRef,
		renderingMenuDataRef,
		objectsMenuDataRef,
		lightsMenuDataRef
	);
	m_ui = new UI::UI(m_window, dataRef);
}

Application::~Application()
{
	delete m_ui;
	delete m_skyboxCubeMap;
	const size_t nModels = m_models.size();
	for (size_t modelIndex = 0; modelIndex < nModels; ++modelIndex) {
		m_models[modelIndex].deleteDrawable();
	}
	m_models.clear();
	const size_t nLights = m_lightSources.size();
	for (size_t lightIndex = 0; lightIndex < nLights; ++lightIndex) {
		m_lightSources[lightIndex].deleteDrawable();
	}
	m_lightSources.clear();
	glfwTerminate();
}

#include "OpenGL/Texture/Texture2D/Texture2D.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"
#include "OpenGL/ErrorHandler.h"
#include "LoadableData/Image/Tools/Import/Import.h"
void Application::run()
{
	if (m_window == nullptr)
		return;

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// Temp
	int maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	Console::print("Max texture size = " + std::to_string(maxTextureSize) + "x" + std::to_string(maxTextureSize) + "\n");
	Console::print("Max vertices that can be raytraced = " + std::to_string(double(maxTextureSize * maxTextureSize) / 8.0) + "\n");

	int textureUnitsFrag = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnitsFrag);
	Console::print("Max textures in frag shader at once = " + std::to_string(textureUnitsFrag) + "\n");

	{ ///START SCOPE		
		Shader shaderSky("res/shaders/Skybox.shader");
		m_skyboxCubeMap->loadFromColor(glm::vec3(0.10f, 0.10f, 0.10f));

		VertexArray vaSky;
		VertexBuffer vbSky(skyboxVertices, 36 * 3 * sizeof(float));
		VertexBufferLayout layoutSky;
		layoutSky.Push<float>(3); // pos
		vaSky.addBuffer(vbSky, layoutSky);

		const glm::mat4 oneM = glm::mat4(1.0f);

		while (!glfwWindowShouldClose(m_window))
		{
			Renderer::clear();

			keysinput(m_window);

			/* Poll for and process events */
			glfwPollEvents();

			m_enableKeysInput = true;
			m_ui->processStatic();

			if (showCursor) {
				m_ui->process();
			}
			if (m_enableVSync != m_lastEnableVSync) {
				Console::print(std::string("VSync ") + (m_enableVSync ? "enabled\n" : "disabled\n"));
				glfwSwapInterval(m_enableVSync);
			}
			m_lastEnableVSync = m_enableVSync;

			//Draw Skybox
			vaSky.bind();
			shaderSky.bind();
			shaderSky.setUniformMatrix4f("u_Model", glm::translate(oneM, glm::vec3(m_camera.getPos().x, m_camera.getPos().y, m_camera.getPos().z)));
			shaderSky.setUniformMatrix4f("u_ViewProj", m_camera.getViewProj());
			shaderSky.setUniform1i("u_skybox", 0);
			m_skyboxCubeMap->bind(0);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			m_skyboxCubeMap->unbind();

			if (m_renderingType == RenderingType::DEFAULT || m_renderingType == RenderingType::DEFERRED) { // Ok for now
				const size_t nLightSources = m_lightSources.size();
				const size_t nModels = m_models.size();
				//
				for (size_t i = 0; i < nLightSources; ++i) {
					// Creating shadow maps
					// Something like this: m_lightSources[i].createShadowMap(m_models);
				}

				for (size_t i = 0; i < nModels; ++i)
					Renderer::draw(m_models[i].getDrawable(), m_models[i].getModelMatrix(), m_camera.getView(), m_camera.getProj());

				for (size_t i = 0; i < nLightSources; ++i)
					Renderer::draw(m_lightSources[i].getDrawable(), m_lightSources[i].getModelMatrix(), m_camera.getView(), m_camera.getProj());

				// Ambient Occlusion

				// POSTFX if there will be any
			}

			Renderer::draw(m_ui);

			glfwSwapBuffers(m_window);

			updateDeltaTime();
		} ///End while
	} /// END SCOPE
}
