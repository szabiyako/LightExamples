#include "MemoryMetric.h"

namespace memory
{
	void printUsage()
	{
		std::cout << "Memory Allocated: " << s_AllocationMetrics.CurrentUsage() << " bytes" << std::endl;
	}
}

void* operator new(size_t size)
{
#ifdef ENABLE_COSOLEOUT_MEMORY_METRIC
	std::cout << "Allocating " << size << " bytes" << std::endl;
#endif
	memory::s_AllocationMetrics.TotalAllocated += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
#ifdef ENABLE_COSOLEOUT_MEMORY_METRIC
	std::cout << "Freeing " << size << " bytes" << std::endl;
#endif
	memory::s_AllocationMetrics.TotalFreed += size;
	free(memory);
}