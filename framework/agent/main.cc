#include "controller.h"
#include <stdlib.h>

xsvrd::Controller g_controller;

int main()
{
	if (g_controller.PrepareToRun() < 0) {
		std::clog << "Controller prepare to run failed!" << std::endl;
		exit(-1);
	}
	g_controller.Run();
	return 0;
}
