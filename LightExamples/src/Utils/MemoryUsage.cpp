#include "MemoryUsage.h"

#include <iostream>
#include <windows.h>

void MemoryUsage::beforeStart()
{
#ifdef MEMORY_METRIC_ENABLE
	std::cout << "Memory usage before start app: ";
	memory::printUsage();
#endif // MEMORY_METRIC_ENABLE
}

void MemoryUsage::afterStart()
{
#ifdef CRT_MEMORY_TEST_ENABLE
	_CrtMemCheckpoint(&initAppState);
#endif // CRT_MEMORY_TEST_ENABLE
#ifdef MEMORY_METRIC_ENABLE
	std::cout << "Memory usage after start app: ";
	memory::printUsage();
#endif // MEMORY_METRIC_ENABLE
}

void MemoryUsage::atClosing()
{
#ifdef CRT_MEMORY_TEST_ENABLE
	_CrtMemState exitFromAppState;
	_CrtMemCheckpoint(&exitFromAppState);
#endif // CRT_MEMORY_TEST_ENABLE
#ifdef MEMORY_METRIC_ENABLE
	std::cout << "Memory usage at closing app: ";
	memory::printUsage();
#endif // MEMORY_METRIC_ENABLE
#ifdef CRT_MEMORY_TEST_ENABLE
	_CrtMemState leaksInApp;
	if (_CrtMemDifference(&leaksInApp, &initAppState, &exitFromAppState)) {
		_CrtMemDumpStatistics(&leaksInApp);
	}
#endif // CRT_MEMORY_TEST_ENABLE
}

void MemoryUsage::afterClosing()
{
#ifdef CRT_MEMORY_TEST_ENABLE
	//_CrtDumpMemoryLeaks();
#endif // CRT_MEMORY_TEST_ENABLE
#ifdef MEMORY_METRIC_ENABLE
	std::cout << "Memory after closing app: ";
	memory::printUsage();
	system("pause");
#endif // MEMORY_METRIC_ENABLE
}
