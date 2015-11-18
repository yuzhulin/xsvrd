#include "../../include/libcommon/x_thread.h"

namespace xsvrd { 

#if defined(_WIN32)
DWORD WINAPI ThreadRoutine(void* para)
#elif defined(__linux__) || defined(TARGET_OS_MAC)
void* ThreadRoutine(void* para)
#endif
{
	if (!para) return NULL;
	Thread* thread = static_cast<Thread*>(para);
	thread->Init();
	if (SUCCESS != thread->PrepareToRun()) {
		thread->SetStatus(TS_STOPPED);
		return NULL;
	}
	thread->SetStatus(TS_RUNNING);
	thread->Routine();
	thread->SetStatus(TS_STOPPED);
	return NULL;
}

Thread::Thread()
{
	status_ = TS_INIT;
}

Thread::~Thread()
{
}

void Thread::Init()
{
	status_ = TS_INIT;
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
	retval = pthread_create(&thread_, &attribute_, ThreadRoutine, this); 
#endif
	return retval;
}

int Thread::PrepareToRun()
{
	int retval = SUCCESS;
	return retval; 
}

int Thread::Routine()
{
	int retval = SUCCESS;
	return retval;
}

bool Thread::IsToBeBlocked()
{
	bool retval = true;
	return retval;
}

int Thread::Block()
{
#if defined(_WIN32)

#elif defined(__linux__) || defined(TARGET_OS_MAC)
	pthread_mutex_lock(&mutex_);
	while (IsToBeBlocked()) {
		status_ = TS_BLOCKED;
		pthread_cond_wait(&cond_, &mutex_);
	}
	pthread_mutex_unlock(&mutex_);
#endif
	return 0;	
}

}

