#include <iostream>
#include "mainsvrd_control.h"

MainsvrdControl g_mainsvrd_control;

int main() 
{
	if (g_mainsvrd_control.PrepareToRun()) {
		std::clog << "Prepare failed!" << std::endl;
		exit(-1);
	}
	g_mainsvrd_control.Run();
	std::clog << "hello !" << std::endl;
	return 0;
}
