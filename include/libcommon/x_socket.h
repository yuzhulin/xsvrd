#ifndef __X_SOCKET__
#define __X_SOCKET__

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../include/os_refactor.h"

// --------------------------------------------------------
int SetSocketOption(SOCKET socket, int level, int optname, const void* optval, int optlen)
{
	int retval = -1;
#ifdef _WIN32
	retval = setsockopt(socket, level, optname,
		static_cast<const char*>(optval), optlen);
#endif

	return retval;
}
int CloseSocket(SOCKET socket)
{
	int retval = -1;
#ifdef _WIN32
	retval = closesocket(socket);
#endif
	retval = close(socket);
	return retval;
}

// --------------------------------------------------------

#define IP_LENGTH 16

enum SocketType {
	ST_COMMON,
	ST_LISTEN
};

enum SocketStatus {
	SS_CLOSED,
	SS_OPENED,
	SS_CONNECTED,
	SS_ERROR,
};

class TCPSocket {
public:
	TCPSocket();
	virtual ~TCPSocket();
	int CreateClient(char* local_addr);
	void Init(unsigned int recv_buf_len);
	void Close();

private:
	SOCKET socket_fd_;
	SocketType socket_type_;
	SocketStatus socket_status_;
	unsigned short server_port_;
	unsigned int server_ip_;
	char server_ip_str_[IP_LENGTH];
	int post_begin_;
	int post_end_;
	unsigned int recv_pack_count_;
	unsigned char* byte_post_buffer_;
};

TCPSocket::TCPSocket() 
{
}

TCPSocket::~TCPSocket() 
{
}

void TCPSocket::Init(unsigned int recv_buf_len)
{
	socket_fd_        = INVALID_SOCKET;
	socket_type_      = ST_COMMON;
	socket_status_    = SS_CLOSED;
	server_port_      = 0;
	post_begin_       = 0;
	post_end_         = 0;
	server_ip_        = 0;
	recv_pack_count_  = 0;
	byte_post_buffer_ = NULL;
	memset(&server_ip_str_, 0, sizeof(server_ip_str_));
}

int TCPSocket::CreateClient(char* local_addr)
{
	if (socket_fd_ != INVALID_SOCKET 
		&& socket_status_ != SS_CLOSED) {
		Close();
	}
	socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socket_fd_) {
		std::clog << "Create socket failed!" << std::endl;
		socket_status_ = SS_CLOSED;
		return -1;
	}
	server_ip_ = 0;	
	server_port_ = 0;
	recv_pack_count_ = 0;
	socket_type_ = ST_COMMON;
	socket_status_ = SS_OPENED;
	memset(server_ip_str_, 0, sizeof(server_ip_str_));
	if (local_addr) {
		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET; 
		addr.sin_addr.s_addr = inet_addr(local_addr);
		bind(socket_fd_, (const struct sockaddr*)(&addr), sizeof(addr));
	}
	//if (setsockopt(socket_fd_, SOL_SOCKET, SO_SNDBUF, 
	return 0;
}

void TCPSocket::Close()
{
	if (socket_fd_ != INVALID_SOCKET) {
		CloseSocket(socket_fd_);
		socket_fd_ = INVALID_SOCKET;
	}
	socket_status_ = SS_CLOSED;
	post_begin_ = post_end_ = 0;
	if (byte_post_buffer_) {
		delete[] byte_post_buffer_;
		byte_post_buffer_ = NULL;
	}
}

#endif
