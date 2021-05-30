#include "DebugMenuDataRef.h"

UI::DebugMenuDataRef::DebugMenuDataRef(
	bool& enableFPScounter,
	bool& enableCursor)
	: enableFPScounter(enableFPScounter),
	  enableCursor(enableCursor)
{
}
