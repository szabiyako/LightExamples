#pragma once

namespace UI {
struct RenderingMenuDataRef
{
	unsigned int &maxFPS;
	bool &enableVSync;
	
	RenderingMenuDataRef(
		unsigned int &maxFPS,
		bool &enableVSync);
};
}
