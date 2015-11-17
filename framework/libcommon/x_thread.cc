#include "../../include/libcommon/x_thread.h"

namespace xsvrd { 

#if defined(_WIN32)
unsigned int Routine(void* para)
#elif defined(__linux__) || defined(TARGET_OS_MAC)
void* Routine(void* para)
#endif
{
	if (!para) return NULL;
	Thread* thread = static_cast<Thread*>(para);
	thread->Init();
	return NULL;
}

Thread::Thread()
{
}

Thread::~Thread()
{
}

void Thread::Init()
{
}

int Thread::Create()
{
	int retval = SUCCESS;
#if defined(_WIN32)

#elif defined(__linux__) || defined(TARGET_OS_MAC) 
	retval = pthread_attr_init(&attribute_);
	if (SUCCESS != retval) {
		std::clog << "pthread_attr_init failed! " 
			<< "errno: " << retval << std::endl;
		return retval;
	}
	retval = pthread_attr_setscope(&attribute_, PTHREAD_SCOPE_SYSTEM);
	if (SUCCESS != retval) {
		std::clog << "pthread_attr_setscope failed! " 
			<< "errno: " << retval << std::endl;
		return retval;
	}
	retval = pthread_attr_setdetachstate(&attribute_, PTHREAD_CREATE_JOINABLE);
	if (SUCCESS != retval) {
		std::clog << "pthread_attr_setdetachstate failed! " 
			<< "errno: " << retval << std::endl;
		return retval;
	}
	retval = pthread_cond_init(&cond_, NULL);
	if (SUCCESS != retval) {
		std::clog << "pthread_cond_init failed! " 
			<< "errno: " << retval << std::endl;
		return retval;
	}
	retval = pthread_mutex_init(&mutex_, NULL);
	if (SUCCESS != retval) {
		std::clog << "pthread_mutex_init failed! " 
			<< "errno: " << retval << std::endl;
		return retval;
	}
	retval = pthread_create(&thread_, &attribute_, Routine, this); 
#endif
	return retval;
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

