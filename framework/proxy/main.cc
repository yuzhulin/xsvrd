#include "proxycontrol.h"
#include <stdlib.h>

xsvrd::ProxyControl g_proxy_control;

int main()
{
	if (g_proxy_control.Init("../cnf/proxy.cnf")) {
		std::clog << "Controller init failed!" << std::endl;
		exit(-1);
	}

	if (g_proxy_control.PrepareToRun()) {
		std::clog << "Controller prepare to run failed!" << std::endl;
		exit(-1);
	}
	g_proxy_control.Run();
	return 0;
}
