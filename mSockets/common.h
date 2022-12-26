/* Headers to be included by the abstract classes */

#pragma once

// IPV4,IPV6 enum
enum IPFamily{
    AF_INET,
    AF_INET6
};


#if defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif // !_WIN32_WINNT
#include <WinSock2.h>
#include <WS2tcpip.h>
// #pragma comment(lib, "ws2_32.lib") // Visual studio linking

// Windows macros 
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

//Macros unix
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#endif