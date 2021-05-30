#pragma once

#include "UI/Element.h"
#include "DataRef.h"

namespace UI {
class UI
{
public:
	UI(GLFWwindow *window, const DataRef &dataRef, const unsigned int glslVersion = 130);
	~UI();

	void draw();

	void setWindowSize(const int width, const int height);
	void process();
	void processStatic();
private:
	void getFrame();

	void initElements(const DataRef& dataRef);
	void initElementsStatic(const DataRef& dataRef);

	bool m_isFrameCreated = false;

	std::vector<Element*> m_elements;
	std::vector<Element*> m_elementsStatic;
	int m_windowWidth = 0;
	int m_windowHeight = 0;
	bool& m_enableKeysInput;
};

}

