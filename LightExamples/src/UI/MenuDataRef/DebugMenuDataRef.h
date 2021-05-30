#pragma once

namespace UI {
struct DebugMenuDataRef
{
	bool &enableFPScounter;
	bool &enableCursor;

	DebugMenuDataRef(
		bool& enableFPScounter,
		bool& enableCursor);
};
}

