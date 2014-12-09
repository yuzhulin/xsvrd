#include "os_refactor.h"
#include "dynamiclibloader.h"
#include "logger_interface.h"

// all global variables.
int32 g_nRunOneInstance = 1;
LoggerInterface* g_loggerinterface = NULL;

void ParseCommand(int32 argc, char** argv);
bool RunOneInstance();

int32 InitLoggerInterface();

int32 main(int32 argc, char** argv)
{
	// ParseCommand(argc, argv);

	InitLoggerInterface();

	// RunOneInstance();

	std::cin.get();
    return 0;
}

int32 InitLoggerInterface()
{
	DynamicLibLoader liblogloader;
	liblogloader.Init("../lib/liblog", DLF_EXTEND_NAME);
	g_loggerinterface = (LoggerInterface*)liblogloader.CreateObjByExportFunction();
	if (!g_loggerinterface) {
		return -1;
	}
	char outputlogpath[MAX_PATH];
	strncpy(outputlogpath, "../log", sizeof(outputlogpath) - 1);
	g_loggerinterface->SetLogPath(outputlogpath);
	g_loggerinterface->SetNormalLogSwitch(SWITCH_OFF);
	g_loggerinterface->SetInfoLogSwitch(SWITCH_ON);
	g_loggerinterface->SetWarnLogSwitch(SWITCH_ON);
	g_loggerinterface->SetErrorLogSwitch(SWITCH_ON);
	g_loggerinterface->SetShowMillisecondSwitch(SWITCH_ON);
	g_loggerinterface->SetInfoLogName("info.log");
	g_loggerinterface->SetWarnLogName("warn.log");
	g_loggerinterface->SetErrorLogName("error.log");
	g_loggerinterface->SetNormalLogName("normal.log");


	SET_INTERFACE_INSTANCE_PTR(g_loggerinterface);

	WRITE_INFO_LOG("this is a test.\n");

	return 0;
}

void ParseCommand(int32 argc, char** argv)
{
	// you can use option "-v" to check the build version
	////////////////////////////////////////////////////////////////
	/*if ((argc > 1) && (!strcasecmp(argv[1], "-v"))) {
		std::clog << "build in [" << __DATE__ << ' ' << __TIME__
			<< "](gcc" << __GNUC__ << '.' << __GNUC_MINOR__
				<< ")" << 8 * sizeof(long) << "bit" << std::endl;
		exit(0);
	}*/
}

bool RunOneInstance()
{
	/*if (!g_nRunOneInstance)
	{
		return true;
	}*/




	// TODO: add logic
	/////////////////////////////////////////////////


	/////////////////////////////////////////////////
    return true;
}
