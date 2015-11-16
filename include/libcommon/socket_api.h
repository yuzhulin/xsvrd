#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__
#include "../os_refactor.h"

int CloseSocket(SOCKET socket);
int Bind(SOCKET socket, const sockaddr* addr, SocketAddressLength addrlen);
int GetSocketOption(SOCKET socket, int level, int optname, void* optval, SocketOptionLength* optlen);
int SetSocketOption(SOCKET socket, int level, int optname, const void* optval, SocketOptionLength optlen);

#endif
