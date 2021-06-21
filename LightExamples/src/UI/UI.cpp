#include "UI.h"

#include <string>

#include "GLFW/glfw3.h"

//ElementsStatic
#include "Elements/Utils/FPSCounter.h"
#include "Elements/Utils/SimpleCursor.h"
#include "Elements/Utils/RayTracingProgressBar.h"

//Elements
#include "Elements/MainMenu.h"

UI::UI::UI(GLFWwindow *window, const DataRef& dataRef, const unsigned int glslVersion)
	: m_enableKeysInput(dataRef.enableKeysInput)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	std::string glslVersionStr = "#version " + std::to_string(glslVersion);
	ImGui_ImplOpenGL3_Init(glslVersionStr.c_str());

	initElementsStatic(dataRef);
	initElements(dataRef);

	glfwGetFramebufferSize(window, &m_windowWidth, &m_windowHeight);
}

UI::UI::~UI()
{
	for (Element* element : m_elements)
		delete element;
	for (Element* element : m_elementsStatic)
		delete element;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::UI::draw()
{
	ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(m_window, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_isFrameCreated = false;
}

void UI::UI::setWindowSize(const int width, const int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}

void UI::UI::process()
{
	getFrame();
	for (Element* element : m_elements)
		element->process(m_windowWidth, m_windowHeight, m_enableKeysInput);
}

void UI::UI::processStatic()
{
	getFrame();
	for (Element* element : m_elementsStatic)
		element->process(m_windowWidth, m_windowHeight, m_enableKeysInput);
}

void UI::UI::getFrame()
{
	if (!m_isFrameCreated) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		m_isFrameCreated = true;
	}
}

void UI::UI::initElements(const DataRef& dataRef)
{
	m_elements.push_back(new MainMenu(dataRef));
}

void UI::UI::initElementsStatic(const DataRef& dataRef)
{
	m_elementsStatic.push_back(new FPSCounter(dataRef.debugMenuDataRef.enableFPScounter));
	m_elementsStatic.push_back(new SimpleCursor(dataRef.debugMenuDataRef.enableCursor));
	m_elementsStatic.push_back(new RayTracingProgressBar(dataRef.renderingMenuDataRef.rayTracer, dataRef.renderingMenuDataRef.renderingType));
}
