#include "../../include/libcommon/x_thread.h"

namespace xsvrd { 

Thread::Thread()
{
}

Thread::~Thread()
{
}

int Thread::Create()
{
#ifdef _WIN32

#elif __linux__
	

#endif
}

int Thread::PrepareToRun()
{
	return 0;
}

int Thread::Run()
{
	return 0;
}

}

