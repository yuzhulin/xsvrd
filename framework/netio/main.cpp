#include <iostream>
#include "../../include/os.h"

void ParseCommand(int32 argc, char** argv);

int32 main(int32 argc, char** argv)
{
	ParseCommand(argc, argv);

	std::cin.get();
    return 0;
}

void ParseCommand(int32 argc, char** argv)
{
	// you can use option "-v" to check the build version
	////////////////////////////////////////////////////////////////
	if ((argc > 1) && (!strcasecmp(argv[1], "-v")))
	{
		std::clog << "build in [" << __DATE__ << ' ' << __TIME__
			<< "](gcc" << __GNUC__ << '.' << __GNUC_MINOR__
				<< ")" << 8 * sizeof(long) << "bit" << std::endl;
	}
	exit(0);
}
