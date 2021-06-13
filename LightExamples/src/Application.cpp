#include "Application.h"

#include "Utils/Console.h"

void Application::updateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrameTime;
	m_lastFrameTime = currentFrame;
}
#include "RayTracing/Utils.h"
void Application::processChanges()
{
	if (m_enableVSync != m_lastEnableVSync) {
		Console::print(std::string("VSync ") + (m_enableVSync ? "enabled\n" : "disabled\n"));
		glfwSwapInterval(m_enableVSync);
	}
	m_lastEnableVSync = m_enableVSync;
	if ((m_renderingType == RenderingType::RAYTRACING)
		&& !m_isBVHcreated) {
		m_isBVHcreated = true;
		//Create BVH
		if (m_bvh != nullptr)
			delete m_bvh;
		m_bvh = new BVH::BVHBuilder();
		Utils::fillBVH(*m_vertexTexture, *m_normalTexture, *m_bvhTexture, *m_bvh, m_models);
	}
	if (m_renderingType != RenderingType::RAYTRACING)
		m_isBVHcreated = false;

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
		m_camera.rotateFromCursor(xpos, ypos, m_cameraSensitivity);
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

		m_camera.rotateFromJoystick(xRotation, yRotation, m_cameraSensitivity);
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
	m_bvhTexture = new Texture::VertexTexture();
	m_vertexTexture = new Texture::VertexTexture();
	m_uvTexture = new Texture::VertexTexture();
	m_normalTexture = new Texture::VertexTexture();

	UI::DebugMenuDataRef debugMenuDataRef(m_enableFPScounter,m_enableCursor);
	UI::CameraMenuDataRef cameraMenuDataRef(m_camera, m_cameraSpeed, m_cameraSensitivity);
	UI::RenderingMenuDataRef renderingMenuDataRef(m_renderingType, m_nRaysMax, m_skyboxImages, *m_skyboxCubeMap, m_enableVSync, m_enableSSAO);
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
	if (m_bvh != nullptr)
		delete m_bvh;
	if (m_bvhTexture != nullptr)
		delete m_bvhTexture;
	if (m_vertexTexture != nullptr)
		delete m_vertexTexture;
	if (m_uvTexture != nullptr)
		delete m_uvTexture;
	if (m_normalTexture != nullptr)
		delete m_normalTexture;
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
	Console::print("Max vertices that can be raytraced = " + std::to_string(double(maxTextureSize * maxTextureSize)) + "\n");
	Console::print("Max triangles that can be raytraced (worst case) = " + std::to_string(double(maxTextureSize * maxTextureSize) / 3.0) + "\n\n");

	int maxTextureArraySize = 0;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTextureArraySize);
	Console::print("Max texture array size = " + std::to_string(maxTextureArraySize) + "\n\n");

	int textureUnitsFrag = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnitsFrag);
	Console::print("Max textures in frag shader at once = " + std::to_string(textureUnitsFrag) + "\n\n");

	Shader rtShader("res/shaders/RayTracing.shader");
	Shader textureShader("res/shaders/TextureTestShader.shader");
	float rtvertices[] = {
		-1, -1,
		1, 1,
		-1, 1,

		-1, -1,
		1, -1,
		1, 1
	};
	VertexArray rtva;
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

			processChanges();

			if (m_renderingType == RenderingType::DEFAULT || m_renderingType == RenderingType::DEFERRED) { // Ok for now

				//Draw Skybox
				vaSky.bind();
				shaderSky.bind();
				shaderSky.setUniformMatrix4f("u_Model", glm::translate(oneM, glm::vec3(m_camera.getPos().x, m_camera.getPos().y, m_camera.getPos().z)));
				shaderSky.setUniformMatrix4f("u_ViewProj", m_camera.getViewProj());
				shaderSky.setUniform1i("u_skybox", 0);
				m_skyboxCubeMap->bind(0);
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
				m_skyboxCubeMap->unbind();

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
			if (m_renderingType == RenderingType::RAYTRACING) {
				const size_t nLightSources = m_lightSources.size();
				const size_t nModels = m_models.size();
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				for (size_t i = 0; i < nModels; ++i)
					Renderer::draw(m_models[i].getDrawable(), m_models[i].getModelMatrix(), m_camera.getView(), m_camera.getProj());
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

				rtva.bind();
				rtShader.bind();
				rtShader.setUniform1i("texPosition", 0);
				rtShader.setUniform1i("texNode", 1);
				rtShader.setUniform1i("u_skybox", 2);
				rtShader.setUniform1i("u_normalTexture", 3);
				rtShader.setUniformMatrix3f("viewToWorld", glm::transpose(m_camera.getView()));
				rtShader.setUniformVec3f("location", m_camera.getPos());
				rtShader.setUniform1f("u_fov", m_camera.getFOV());
				int width, height;
				glfwGetWindowSize(m_window, &width, &height);
				rtShader.setUniformVec2f("screeResolution", glm::vec2(width, height));
				rtShader.setUniform1i("bvhWidth", m_bvhTexture->getWidth());
				rtShader.setUniform1i("texPosWidth", m_vertexTexture->getWidth());

				//setupLight
				std::vector<glm::vec3> lightPos;
				lightPos.reserve(32);
				for (size_t i = 0; i < nLightSources; ++i) {
					if (m_lightSources[i].isEnabled())
						lightPos.push_back(m_lightSources[i].getPos());
				}
				rtShader.setUniform1i("u_nPointLights", lightPos.size());
				rtShader.setUniformVec3fArray("u_pointLightsPos", lightPos, 32);
				rtShader.setUniform1i("u_nRaysMax", m_nRaysMax);

				m_vertexTexture->bind(0);
				m_bvhTexture->bind(1);
				m_skyboxCubeMap->bind(2);
				m_normalTexture->bind(3);
				// Draw
				glDisable(GL_DEPTH_TEST);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glEnable(GL_DEPTH_TEST);

				for (size_t i = 0; i < nLightSources; ++i)
					Renderer::draw(m_lightSources[i].getDrawable(), m_lightSources[i].getModelMatrix(), m_camera.getView(), m_camera.getProj());
			}

			Renderer::draw(m_ui);

			glfwSwapBuffers(m_window);

			updateDeltaTime();
		} ///End while
	} /// END SCOPE
}
