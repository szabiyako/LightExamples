#pragma once

//#define MEMORY_METRIC_ENABLE
//#define CRT_MEMORY_TEST_ENABLE

#ifdef MEMORY_METRIC_ENABLE
#include "Utils/MemoryMetric/MemoryMetric.h"
#endif // MEMORY_METRIC_ENABLE


#ifdef CRT_MEMORY_TEST_ENABLE
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // CRT_MEMORY_TEST_ENABLE

namespace MemoryUsage {
	void beforeStart();
	void afterStart();
	void atClosing();
	void afterClosing();

#ifdef CRT_MEMORY_TEST_ENABLE
	static _CrtMemState initAppState;
	static _CrtMemState exitFromAppState;
	static _CrtMemState leaksInApp;;
#endif // CRT_MEMORY_TEST_ENABLE
}