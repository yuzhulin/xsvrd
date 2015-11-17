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
#if defined(_WIN32)

#elif defined(__linux__) || defined(TARGET_OS_MAC) 
	pthread_attr_init(&attribute_);
	//pthread_attr_setscope
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

