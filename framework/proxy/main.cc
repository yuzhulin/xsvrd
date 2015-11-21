#include "controller.h"
#include <stdlib.h>

xsvrd::Controller g_controller;

int main()
{
	if (g_controller.Init()) {
		std::clog << "Controller init failed!" << std::endl;
		exit(-1);
	}

	if (g_controller.PrepareToRun()) {
		std::clog << "Controller prepare to run failed!" << std::endl;
		exit(-1);
	}
	g_controller.Run();
	return 0;
}
