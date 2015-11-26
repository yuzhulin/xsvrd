#include <fstream>
#include "mainsvrd_control.h"

MainsvrdControl::MainsvrdControl()
{
}

MainsvrdControl::~MainsvrdControl()
{
}

int MainsvrdControl::ReadConfigFile()
{
	std::string config_file = get_config_file();
	std::ifstream ifs(config_file.c_str());
	if (!ifs) return -1;
	
	
	return 0;
}

int MainsvrdControl::Init() 
{
	std::string config_file("../cnf/core.cnf");
	set_config_file(config_file);
	return Control::Init();
}

int MainsvrdControl::PrepareToRun()
{
	return 0;
}

int MainsvrdControl::Run()
{
	return 0;
}


