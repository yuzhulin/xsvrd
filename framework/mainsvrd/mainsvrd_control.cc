#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>
#include "mainsvrd_control.h"

#if !defined(_WIN32)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

MainsvrdControl::MainsvrdControl()
{
	configuration_.id = 0;
	configuration_.port = 0;
	configuration_.ip.clear();
}

MainsvrdControl::~MainsvrdControl()
{
}

int MainsvrdControl::ReadConfigFile()
{
	std::string config_file = get_config_file();
	std::ifstream ifs(config_file.c_str());
	if (!ifs) return -1;
	std::vector<std::string> keys;
	keys.push_back("ID");
	keys.push_back("IP");
	keys.push_back("Port");
	std::map<std::string, std::string> key_value_map;	
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
		if ("ID" == *it) {
			value_int = atoi(value.c_str());
			if (value_int <= 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.id = value_int;
		}
		if ("IP" == *it) {
			if (!inet_aton(value.c_str(), NULL)) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.ip = value;
		}
		if ("Port" == *it) {
			value_int = atoi(value.c_str());
			if (value_int <= 0) {
				std::clog << config_file << " --> " << "\"" 
					<< *it << "\"" << " invalid." << std::endl; 
				return -1;
			}
			configuration_.port 
				= static_cast<unsigned short>(value_int);
		}
	}
#ifdef XDBG
	std::clog << "\"" << config_file << "\"" << std::endl;
	std::clog << "--------------------------" << std::endl;
	std::clog << "ID: " << configuration_.id << std::endl;
	std::clog << "IP: " << configuration_.ip << std::endl;
	std::clog << "Port: " << configuration_.port << std::endl;
	std::clog << "--------------------------" << std::endl << std::endl;
#endif
	return 0;
}

int MainsvrdControl::Init() 
{
	std::string config_file("../cnf/core.cnf");
	set_config_file(config_file);
	return Control::Init();
}

int MainsvrdControl::PrepareToRun()
{
	return 0;
}

int MainsvrdControl::Run()
{
	return 0;
}


