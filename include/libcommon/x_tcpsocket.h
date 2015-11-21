#ifndef __X_TCPSOCKET__
#define __X_TCPSOCKET__

#include <cstring>

#include "../../include/os_refactor.h"
#include "../../include/libcommon/socket_api.h"

#define SEND_BUFFER_SIZE     (260 * 1024 - 2)
#define RECV_BUFFER_SIZE     (512 * 1024)
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
	int CreateClient(const char* addr = NULL);
	int CreateServer(unsigned short port, const char* addr = NULL);
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
	int read_begin_;
	int read_end_;
	unsigned int recv_pack_count_;
	unsigned char* byte_post_buffer_;
};

TCPSocket::TCPSocket() 
{
	Init(RECV_BUFFER_SIZE);
}

TCPSocket::~TCPSocket() 
{
}

void TCPSocket::Init(unsigned int recv_buf_len)
{
	post_begin_       = 0;
	post_end_         = 0;
	read_begin_       = 0;
	read_end_         = 0;
	server_ip_        = 0;
	server_port_      = 0;
	recv_pack_count_  = 0;
	byte_post_buffer_ = NULL;
	socket_type_      = ST_COMMON;
	socket_status_    = SS_CLOSED;
	socket_fd_        = INVALID_SOCKET;
	memset(&server_ip_str_, 0, sizeof(server_ip_str_));
}

int TCPSocket::CreateServer(unsigned short port, const char* addr)
{
	if (socket_fd_ != INVALID_SOCKET 
		&& socket_status_ != SS_CLOSED) {
		Close();
	}
	if (INVALID_SOCKET == (socket_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
		std::clog << "Create socket failed!" << std::endl;
		socket_status_ = SS_CLOSED;
		return -1;
	}
	int option_value = 1;
	SocketOptionLength option_len = sizeof(option_value);
	if (SOCKET_ERROR == SetSocketOption(socket_fd_, SOL_SOCKET, 
		SO_REUSEADDR, &option_value, option_len)) {
		std::clog << "Set socket addr reuse failed, " 
			<< strerror(errno) << std::endl;
	}
	sockaddr_in addrinfo;
	memset(&addrinfo, 0, sizeof(addrinfo));
	addrinfo.sin_family = AF_INET;
	addrinfo.sin_port = htons(port);
	if (addr) {
		addrinfo.sin_addr.s_addr = inet_addr(addr);
	} else {
		addrinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	if (SOCKET_ERROR == Bind(socket_fd_,
		(const sockaddr*)&addrinfo, sizeof(addrinfo))) {
		std::clog << "Bind addr failed, "
			<< strerror(errno) << std::endl;
		Close();
		return -1;
	}
	if (SOCKET_ERROR == Listen(socket_fd_, 5)) {
		std::clog << "Listen failed, "
			<< strerror(errno) << std::endl;
		Close();
		return -1;
	}
	socket_type_ = ST_LISTEN;
	socket_status_ = SS_OPENED;
	// TODO: 
	// SetNonBlock();
	return 0;
}

int TCPSocket::CreateClient(const char* addr)
{
	if (socket_fd_ != INVALID_SOCKET 
		&& socket_status_ != SS_CLOSED) {
		Close();
	}
	if (INVALID_SOCKET == (socket_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
		std::clog << "Create socket failed!" << std::endl;
		socket_status_ = SS_CLOSED;
		return -1;
	}
	int option_value = SEND_BUFFER_SIZE;
	SocketOptionLength option_len = sizeof(option_value);
	if (SOCKET_ERROR == SetSocketOption(socket_fd_, SOL_SOCKET,
		SO_SNDBUF, &option_value, option_len)) {
		std::clog << "Set socket's send buffer size to " 
	 		<< option_value << " failed!" << std::endl;
		return -1;
	}
	if (SUCCESS == GetSocketOption(socket_fd_, SOL_SOCKET,
		SO_SNDBUF, &option_value, &option_len)) {
		std::clog << "Socket's send buffer is "
			<< option_value << std::endl;
	}
	server_ip_ = 0;	
	server_port_ = 0;
	recv_pack_count_ = 0;
	if (addr) {
		sockaddr_in addrinfo;
		memset(&addrinfo, 0, sizeof(addrinfo));
		addrinfo.sin_family = AF_INET; 
		addrinfo.sin_addr.s_addr = inet_addr(addr);
		if (SOCKET_ERROR == Bind(socket_fd_, (const sockaddr*)&addrinfo, sizeof(addrinfo))) {
			std::clog << "Bind " << addr << " failed!" << std::endl;
		}
	}
	socket_type_ = ST_COMMON;
	socket_status_ = SS_OPENED;
	read_begin_ = read_end_ = 0;
	post_begin_ = post_end_ = 0;
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
