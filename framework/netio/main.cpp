#include <iostream>
#include "../../include/os.h"

int32 g_nRunOneInstance = 1;

void ParseCommand(int32 argc, char** argv);
bool RunOneInstance();

int32 main(int32 argc, char** argv)
{
	ParseCommand(argc, argv);
	RunOneInstance();










	std::cin.get();
    return 0;
}

void ParseCommand(int32 argc, char** argv)
{
	// you can use option "-v" to check the build version
	////////////////////////////////////////////////////////////////
	if ((argc > 1) && (!strcasecmp(argv[1], "-v"))) {
		std::clog << "build in [" << __DATE__ << ' ' << __TIME__
			<< "](gcc" << __GNUC__ << '.' << __GNUC_MINOR__
				<< ")" << 8 * sizeof(long) << "bit" << std::endl;
		exit(0);
	}
}

bool RunOneInstance()
{
	if (!g_nRunOneInstance)
	{
		return true;
	}
	// TODO: add logic
	/////////////////////////////////////////////////


	/////////////////////////////////////////////////
}
