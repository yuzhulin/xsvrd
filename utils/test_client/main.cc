#include "../../include/os_refactor.h"
#include "../../include/libcommon/x_base.h"
#include <iostream>
#include <string.h>

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addrinfo;
	memset(&addrinfo, 0, sizeof(addrinfo));
	addrinfo.sin_family = AF_INET;
	addrinfo.sin_port = htons(12000);
	addrinfo.sin_addr.s_addr = inet_addr("10.211.55.14");
	connect(sockfd, (sockaddr*)&addrinfo, sizeof(addrinfo));

	// std::string str = "hello, xsvrd !";
	// send(sockfd, str.c_str(), str.size(), 0);
	ConnectServerInfo csi;
	csi.server_id = htonl(1024);
	csi.entity_type = htonl(1);
	send(sockfd, &csi, sizeof(csi), 0);

	std::cin.get();
	return 0;
}
