#pragma once

#include "UI/Element.h"

namespace UI {

class FPSCounter : public Element
{
public:
	FPSCounter(bool &show);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
private:
	ImGuiWindowFlags m_windowFlags = 0;

	bool &m_show;
};

}

