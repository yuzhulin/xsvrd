#include "mainsvrd_control.h"

MainsvrdControl::MainsvrdControl()
{
}

MainsvrdControl::~MainsvrdControl()
{
}

int MainsvrdControl::ReadConfigFile()
{
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


