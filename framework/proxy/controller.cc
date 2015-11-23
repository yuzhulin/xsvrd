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
	while (1) {

		CheckConnectRequest();

	}
	return 0;
}

int Controller::Init(std::string config_file)
{
	if (ReadConfiguration(config_file)) {
		return -1;
	}
	return 0;
}

int Controller::CheckConnectRequest()
{
	int retval = 0;
	SOCKET listen_socket_fd = listen_socket_.GetSocketFD(); 
	int fd_max = listen_socket_fd + 1;
	fd_set fd_reads;
	FD_ZERO(&fd_reads);
	FD_SET(listen_socket_fd, &fd_reads);
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 50 * 1000;
	retval = select(fd_max, &fd_reads, NULL, NULL, &timeout); 
	if (retval <= 0) {
		if (retval < 0) {
			std::clog << "select error, " 
				<< strerror(errno) << std::endl;
		}
		return -1;
	}
	sockaddr_in addrinfo;
	SocketAddressLength addrlen = 0;
	memset(&addrinfo, 0, sizeof(addrinfo));
	SOCKET client_socket_fd 
		= accept(listen_socket_fd, (sockaddr*)&addrinfo, &addrlen); 
	if (INVALID_SOCKET == client_socket_fd) {
		return -1;
	}
	timeout.tv_sec = 0;
	timeout.tv_usec = 500 * 1000;
	FD_ZERO(&fd_reads);
	FD_SET(client_socket_fd, &fd_reads);
	fd_max = client_socket_fd + 1;
	retval = select(fd_max, &fd_reads, NULL, NULL, &timeout);
	if (retval < 0) {
		CloseSocket(client_socket_fd);
		return -1;
	}
	ConnectServerInfo connect_server_info;
	int info_code_len = sizeof(connect_server_info);
	memset(&connect_server_info, 0, info_code_len);
	int recv_code_len = recv(client_socket_fd, 
		&connect_server_info, info_code_len, 0); 
	if (info_code_len != recv_code_len) {
		CloseSocket(client_socket_fd);
		return -1;
	}

	std::clog << connect_server_info.entity_type 
		<< " " << connect_server_info.server_id << std::endl;
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
