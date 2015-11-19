#include "handlerthread.h"

namespace xsvrd {

HandlerThread::HandlerThread()
{
}

HandlerThread::~HandlerThread()
{
}

int HandlerThread::PrepareToRun()
{
	return 0;
}

int HandlerThread::Routine()
{
	return 0;
}

bool HandlerThread::IsToBeBlocked()
{
	return true;
}

}
