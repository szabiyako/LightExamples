#include "Utils/MemoryUsage.h"

//#define OGL_DEBUGMODE

#include "Application.h"

//To show console comment out line below
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{
	{
		MemoryUsage::beforeStart();
		Application app;
		MemoryUsage::afterStart();
		app.run();
		MemoryUsage::atClosing();
	}
	MemoryUsage::afterClosing();
	return 0;
}