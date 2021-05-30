#ifndef MEMORY_METRIC_H
#define MEMORY_METRIC_H

#include <iostream>

//comment to hide console info about malloc and free
//#define ENABLE_COSOLEOUT_MEMORY_METRIC
namespace memory
{
	struct AllocationMetrics
	{
		uint32_t TotalAllocated = 0;
		uint32_t TotalFreed = 0;

		uint32_t CurrentUsage()
		{
			return TotalAllocated - TotalFreed;
		}
	};

	static AllocationMetrics s_AllocationMetrics;

	void printUsage();
}

void* operator new(size_t size);

void operator delete(void* memory, size_t size);

#endif
