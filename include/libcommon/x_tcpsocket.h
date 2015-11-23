#ifndef __X_TCPSOCKET__
#define __X_TCPSOCKET__

#include <cstring>

#include "../../include/os_refactor.h"
#include "../../include/libcommon/socket_api.h"

#define SEND_BUFFER_SIZE     (260 * 1024 - 2)
#define RECV_BUFFER_SIZE     (512 * 1024)
#define IP_LENGTH 16

namespace xsvrd {

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
	SOCKET GetSocketFD() { return socket_fd_; }

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

}

#endif
