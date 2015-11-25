#include "controller.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>

namespace xsvrd {

Controller::Controller()
{
	dbsvrd_handler_thread_ = NULL;
	mainsvrd_handler_thread_ = NULL;
	othersvrd_handler_thread_ = NULL;
	// TODO:
	// memset(&configuration_, 0, sizeof(configuration_));
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
	connect_server_info.server_id 
		= ntohl(connect_server_info.server_id);
	connect_server_info.entity_type 
		= ntohl(connect_server_info.entity_type);
#ifdef XDBG
	unsigned short port = ntohs(addrinfo.sin_port);
	std::string ip = inet_ntoa(addrinfo.sin_addr);
	std::clog << "Entity(" << connect_server_info.entity_type
		<< ", " << connect_server_info.server_id << ") register ok, " 
			<< "addr is (" << ip << ":" << port << ")" << std::endl; 
#endif
	return 0;
}

int Controller::ReadEntityList(ConnectionEntityType type, TCPConnection* connection, int num)
{
	if (0 == num) return 0;
	if (0 > num) return -1;
	if (!connection) return -1;
	if (num > MAX_SVRD_NUM)
		num = MAX_SVRD_NUM;
	switch (type) {
		case CET_DBSVRD: {
			std::string list_file = configuration_.svrdlist[type].list_file; 
			std::ifstream ifs(list_file.c_str());
			if (!ifs) {
				std::clog << "Open " 
					<< list_file << " failed!" << std::endl;
				return -1;
			}
			std::string id;
			std::string ip;
			std::string line;
			while (std::getline(ifs, line)) {
				std::stringstream ss(line);
				line.clear();
				id.clear();
				ip.clear();
				ss >> id >> ip; 
			}
			break;
		}
		default:
			break;
	}
	return 0;
}

int Controller::ReadConfiguration(std::string config_file)
{
	std::ifstream ifs(config_file.c_str());
	if (!ifs) return -1;
	std::vector<std::string> keys;	
	keys.push_back("ProxyID");
	keys.push_back("ProxyPort");
	keys.push_back("DBSvrdNum");
	keys.push_back("MainSvrdNum");
	keys.push_back("OtherSvrdNum");
	keys.push_back("DBSvrdsList");
	keys.push_back("MainSvrdsList");
	keys.push_back("OtherSvrdsList");
	std::map<std::string, std::string> key_value_map;
	for (std::vector<std::string>::iterator it = keys.begin();
		it != keys.end(); ++it) {
		key_value_map[*it] = "";
	}
	std::string key;
	std::string value;
	std::string line;
	while (std::getline(ifs, line)) {
		std::stringstream ss(line);
		key.clear();
		value.clear();
		ss >> key >> value;
		key_value_map[key] = value;	
	}
	ifs.close();
	for (std::vector<std::string>::iterator it = keys.begin();
		it != keys.end(); ++it) {
		value = key_value_map[*it]; 
		if (value.empty()) {
			std::clog << config_file << " --> " << "\"" 
				<< *it << "\"" << " invalid." << std::endl; 
			return -1;
		}
		int value_int = 0;
		if ("ProxyID" == *it) {
			value_int = atoi(value.c_str());
			if (value_int <= 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.proxy_id = value_int;
		}
		if ("ProxyPort" == *it) {
			value_int = atoi(value.c_str());
			if (value_int <= 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.proxy_port 
				= static_cast<unsigned short>(value_int);
		}
		if ("DBSvrdNum" == *it) {
			value_int = atoi(value.c_str());
			if (value_int < 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.svrdlist[CET_DBSVRD].svrdnum = value_int;
		}
		if ("MainSvrdNum" == *it) {
			value_int = atoi(value.c_str());
			if (value_int < 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.svrdlist[CET_MAINSVRD].svrdnum = value_int;
		}	
		if ("OtherSvrdNum" == *it) {
			value_int = atoi(value.c_str());
			if (value_int < 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.svrdlist[CET_OTHERSVRD].svrdnum = value_int;
		}
		if ("DBSvrdsList" == *it)
			configuration_.svrdlist[CET_DBSVRD].list_file = value;
		if ("MainSvrdsList" == *it)
			configuration_.svrdlist[CET_MAINSVRD].list_file = value;
		if ("OtherSvrdsList" == *it)
			configuration_.svrdlist[CET_OTHERSVRD].list_file = value;
	}
	if (configuration_.svrdlist[CET_DBSVRD].svrdnum)
		ReadEntityList(CET_DBSVRD, dbsvrd_connection_, 
			configuration_.svrdlist[CET_DBSVRD].svrdnum);
	if (configuration_.svrdlist[CET_MAINSVRD].svrdnum)
		ReadEntityList(CET_MAINSVRD, mainsvrd_connection_, 
			configuration_.svrdlist[CET_MAINSVRD].svrdnum);
	if (configuration_.svrdlist[CET_OTHERSVRD].svrdnum)
		ReadEntityList(CET_OTHERSVRD, othersvrd_connection_, 
			configuration_.svrdlist[CET_OTHERSVRD].svrdnum);
#ifdef XDBG
	std::clog << "proxy config: " << std::endl;
	std::clog << "--------------------------" << std::endl;
	std::clog << "ProxyID: " << configuration_.proxy_id << std::endl;
	std::clog << "ProxyPort: " << configuration_.proxy_port << std::endl;
	std::clog << "DBSvrdNum: " 
		<< configuration_.svrdlist[CET_DBSVRD].svrdnum << std::endl;
	std::clog << "MainSvrdNum: " 
		<< configuration_.svrdlist[CET_MAINSVRD].svrdnum << std::endl;
	std::clog << "OtherSvrdNum: " 
		<< configuration_.svrdlist[CET_OTHERSVRD].svrdnum << std::endl;
	std::clog << "DBSvrdsList: " << configuration_.svrdlist[CET_DBSVRD].list_file << std::endl;
	std::clog << "MainSvrdsList: " << configuration_.svrdlist[CET_MAINSVRD].list_file << std::endl;
	std::clog << "OtherSvrdsList: " << configuration_.svrdlist[CET_OTHERSVRD].list_file << std::endl;
	std::clog << "--------------------------" << std::endl;
#endif
	return 0;
}

}
