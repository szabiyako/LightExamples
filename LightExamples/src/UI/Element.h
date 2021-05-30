#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include "imguiFileDialog/ImGuiFileDialog.h"

//#include "Utils/NonCopyable.h"

namespace UI {

class Element// : public NonCopyable
{
public:
	virtual ~Element();
	virtual void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) = 0;
};

}

