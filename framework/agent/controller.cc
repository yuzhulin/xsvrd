#include "controller.h"
#include <iostream>

namespace xsvrd {

Controller::Controller()
{
	dbsvrd_handler_thread_ = NULL;
	mainsvrd_handler_thread_ = NULL;
	othersvrd_handler_thread_ = NULL;
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
	return 0;
}

int Controller::Run()
{
	return 0;
}

}
