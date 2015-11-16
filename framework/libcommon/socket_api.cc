#include "../../include/libcommon/socket_api.h"

int CloseSocket(SOCKET socket)
{
	int retval = SOCKET_ERROR;
#ifdef _WIN32
	retval = closesocket(socket);
#elif __linux__
	retval = close(socket);
#endif
	return retval;
}

int Bind(SOCKET socket, const sockaddr* addr, SocketAddressLength addrlen)
{
	return bind(socket, addr, addrlen);
}

int GetSocketOption(SOCKET socket, int level, int optname, void* optval, SocketOptionLength* optlen)
{
	int retval = SOCKET_ERROR;
#ifdef _WIN32
	retval = getsockopt(socket, level, optname,
 			static_cast<char*>(optval), optlen);
#elif __linux__
	retval = getsockopt(socket, level, optname, optval, optlen); 
#endif
	return retval;
}

int SetSocketOption(SOCKET socket, int level, int optname, const void* optval, SocketOptionLength optlen)
{
	int retval = SOCKET_ERROR;
#ifdef _WIN32
	retval = setsockopt(socket, level, optname,
			static_cast<const char*>(optval), optlen);
#elif __linux__
	retval = setsockopt(socket, level, optname, optval, optlen);
#endif
	return retval;
}
