#include <iostream>
#include "os.h"

#include "ilogger.h"
#include "logger_interface.h"
#include "dynamiclibloader.h"


int32 g_nRunOneInstance = 1;

void ParseCommand(int32 argc, char** argv);
bool RunOneInstance();



int32 main(int32 argc, char** argv)
{
	ParseCommand(argc, argv);
	
	// load confer dll
	///////////////////////////////////////////
	/*HMODULE pDllHandle = LoadLibrary("../modules/mod_logger.dll");
	if (pDllHandle)
	{
		char szFunctionName[100] = "CreateLogFile";
		CreateLogFileProc pfCrateLogFile = NULL;
		pfCrateLogFile = (CreateLogFileProc)(GetProcAddress(pDllHandle, szFunctionName));
	}*/

	DynamicLibLoader liblogloader;
	liblogloader.Init("../lib/liblog.dll");
	LoggerInterface* logger_interface = (LoggerInterface*)liblogloader.CreateObjByExportFunction();
	if (logger_interface) {
		logger_interface->SetLogPath(NULL);
	}
	///////////////////////////////////////////
	
	
	
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
    return true;
}
