#include "controller.h"
#include <iostream>
#include <fstream>

namespace xsvrd {

Controller::Controller()
{
	dbsvrd_handler_thread_ = NULL;
	mainsvrd_handler_thread_ = NULL;
	othersvrd_handler_thread_ = NULL;
	dbsvrd_connection_ = NULL;
	mainsvrd_connection_ = NULL;
	othersvrd_connection_ = NULL;
	memset(&configuration_, 0, sizeof(configuration_));
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
	if (listen_socket_.CreateServer(configuration_.proxy_port)) {
		std::clog << "Create server failed!" << std::endl;
		return -1;
	}
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
	dbsvrd_handler_thread_->Init(CET_DBSVRD, dbsvrd_connection_, 
		mainsvrd_connection_, othersvrd_connection_, &configuration_);
	mainsvrd_handler_thread_->Init(CET_MAINSVRD, dbsvrd_connection_, 
		mainsvrd_connection_, othersvrd_connection_, &configuration_);
	othersvrd_handler_thread_->Init(CET_OTHERSVRD, dbsvrd_connection_, 
		mainsvrd_connection_, othersvrd_connection_, &configuration_);
	if (dbsvrd_handler_thread_->Create()) {
		return -1;
	}
	if (mainsvrd_handler_thread_->Create()) {
		return -1;
	}
	if (othersvrd_handler_thread_->Create()) {
		return -1;
	}
	return 0;
}

int Controller::Run()
{
	std::clog << "run ok!" << std::endl;
	return 0;
}

int Controller::Init(std::string config_file)
{
	if (ReadConfiguration(config_file)) {
		return -1;
	}
	return 0;
}

int Controller::ReadConfiguration(std::string config_file)
{
	std::ifstream ifs(config_file.c_str());
	if (!ifs) {
		return -1;
	}
	ifs >> configuration_.proxy_id >> configuration_.proxy_port;
	ifs.close();
	return 0;
}

}
