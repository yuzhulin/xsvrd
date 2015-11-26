#include "proxycontrol.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>

namespace xsvrd {

ProxyControl::ProxyControl()
{
	dbsvrd_handler_thread_ = NULL;
	mainsvrd_handler_thread_ = NULL;
	othersvrd_handler_thread_ = NULL;
	// TODO:
	// memset(&configuration_, 0, sizeof(configuration_));
}

ProxyControl::~ProxyControl()
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

int ProxyControl::PrepareToRun()
{
	if (listen_socket_.CreateServer(configuration_.proxy_port)) {
		std::clog << "Create server failed!" << std::endl;
		return -1;
	}
	dbsvrd_handler_thread_ = new Handler;	
	if (!dbsvrd_handler_thread_) {
		std::clog << "New dbsvrd_handler_thread failed!" << std::endl;
		return -1;
	}
	mainsvrd_handler_thread_ = new Handler;	
	if (!mainsvrd_handler_thread_) {
		std::clog << "New mainsvrd_handler_thread failed!" << std::endl;
		return -1;
	}
	othersvrd_handler_thread_ = new Handler;	
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

int ProxyControl::Run()
{
	while (1) {

		CheckConnectRequest();

	}
	return 0;
}

int ProxyControl::Init()
{
	std::string config_file("../cnf/core.cnf");
	set_config_file(config_file);
	return Control::Init();
}

int ProxyControl::CheckConnectRequest()
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

int ProxyControl::ReadEntityList(ConnectionEntityType type, TCPConnection* connection)
{
	if (!connection) return -1;
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
	std::vector<std::string> list_order;
	std::map<std::string, std::string> key_value_map;
	while (std::getline(ifs, line)) {
		std::stringstream ss(line);
		line.clear();
		id.clear();
		ip.clear();
		ss >> id >> ip; 
		if (id.empty() || ip.empty()) {
			std::clog << list_file << " " 
				<< "there is a record invalid." << std::endl;
			return -1;
		}
		if (atoi(id.c_str()) > MAX_SVRD_NUM) {
			std::clog << list_file << " " 
				<< "there is an id > max(" 
					<< MAX_SVRD_NUM << ")" << std::endl; 
			return -1;
		}
		if (!key_value_map[id].empty()) {
			std::clog << list_file << " " 
				<< "id(" << id << ") duplicate." << std::endl;
			return -1;
		}
		list_order.push_back(id);
		key_value_map[id] = ip;
	}
	int expect_num = configuration_.svrdlist[type].svrdnum;
	if (key_value_map.size() < expect_num) {
		std::clog << list_file << " " 
			<< "actual num < expect num." << std::endl;
		return -1;
	}
#ifdef XDBG
	std::clog << "\"" << list_file << "\"" << std::endl; 	
	std::clog << "--------------------------" << std::endl;
	for (int i = 0; i < expect_num; i++) {
		std::clog << list_order[i] << " " 
			<< key_value_map[list_order[i]] << std::endl;
	}
	std::clog << "--------------------------" << std::endl << std::endl;
#endif
	int entity_id = 0;
	std::string entity_addr;
	for (int i = 0; i < expect_num; i++) {
		entity_id = atoi(list_order[i].c_str());
		entity_addr = key_value_map[list_order[i]];
		connection[entity_id].Init(type, entity_id, entity_addr, 0);
	}
	return 0;
}

int ProxyControl::ReadConfigFile()
{
	std::string config_file = get_config_file();
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
			if (value_int > MAX_SVRD_NUM) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid, max value is " 
						<< MAX_SVRD_NUM << "." << std::endl; 
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
			if (value_int > MAX_SVRD_NUM) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid, max value is " 
						<< MAX_SVRD_NUM << "." << std::endl; 
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
			if (value_int > MAX_SVRD_NUM) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid, max value is " 
						<< MAX_SVRD_NUM << "." << std::endl; 
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
		if (ReadEntityList(CET_DBSVRD, dbsvrd_connection_)) 
			return -1;
	if (configuration_.svrdlist[CET_MAINSVRD].svrdnum)
		if (ReadEntityList(CET_MAINSVRD, mainsvrd_connection_))
			return -1;
	if (configuration_.svrdlist[CET_OTHERSVRD].svrdnum)
		if (ReadEntityList(CET_OTHERSVRD, othersvrd_connection_))
			return -1;
#ifdef XDBG
	std::clog << "\"" << config_file << "\"" << std::endl;
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
	std::clog << "--------------------------" << std::endl << std::endl;
#endif
	return 0;
}

}
