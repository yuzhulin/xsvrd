#include "controller.h"
#include <iostream>

namespace xsvrd {

Controller::Controller()
{
	dbsvrd_handler_thread_ = NULL;
	mainsvrd_handler_thread_ = NULL;
	othersvrd_handler_thread_ = NULL;
	dbsvrd_connection_ = NULL;
	mainsvrd_connection_ = NULL;
	othersvrd_connection_ = NULL;
}

Controller::~Controller()
{
	if (dbsvrd_handler_thread_) {
		delete dbsvrd_handler_thread_;
		dbsvrd_handler_thread_ = NULL;
	}
	if (mainsvrd_handler_thread_) {
		delete mainsvrd_handler_thread_;
		mainsvrd_handler_thread_ = NULL;
	}
	if (othersvrd_handler_thread_) {
		delete othersvrd_handler_thread_;
		othersvrd_handler_thread_ = NULL;
	}
}

int Controller::PrepareToRun()
{
	dbsvrd_handler_thread_ = new HandlerThread;	
	if (!dbsvrd_handler_thread_) {
		std::clog << "New dbsvrd_handler_thread failed!" << std::endl;
		return -1;
	}
	mainsvrd_handler_thread_ = new HandlerThread;	
	if (!mainsvrd_handler_thread_) {
		std::clog << "New mainsvrd_handler_thread failed!" << std::endl;
		return -1;
	}
	othersvrd_handler_thread_ = new HandlerThread;	
	if (!othersvrd_handler_thread_) {
		std::clog << "New othersvrd_handler_thread failed!" << std::endl;
		return -1;
	}
	return 0;
}

int Controller::Run()
{
	std::clog << "run ok!" << std::endl;
	return 0;
}

}
