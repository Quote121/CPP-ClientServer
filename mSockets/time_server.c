/* time_server.c */

#include <stdio.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif // !_WIN32_WINNT
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Macros Windows
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else // ndef _WIN32
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

// Vista and later support duel stack sockets, however many windows sockets
// are missing the definition
#ifndef IPV6_ONLY
#define IPV6_V6ONLY 27
#endif


int init_socket(WSADATA* d, SOCKET* s){
	// Set the windows socket version
#if defined(_WIN32)
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
		fprintf(stderr, "Failed to initalize.\n");
		return 1;
	}
#endif

	// Configure local address
	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	/* AF_INET = ipv4 */
	hints.ai_family = AF_INET6;
	/* TCP, soCK_DGRAM = UDP */
	hints.ai_socktype = SOCK_STREAM;
	/* Tells getaddrinfo() to bind to the wildcard address */
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* bind_address;
	/* getaddrinfo here generates an address for bind() */
	/* to do this we pass null as first param */
	getaddrinfo(0, "8080", &hints, &bind_address);



}





int main() {




	printf("Creating socket...\n");
	SOCKET socket_listen;
	socket_listen = socket(bind_address->ai_family,
		bind_address->ai_socktype, bind_address->ai_protocol);

	/* check if the call to socket() was sucessful */
	if (!ISVALIDSOCKET(socket_listen)) {
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	/* implementation of duel stack ipv4 and ipv6 */
	/* when an ipv4 connection connects the ipv4 is remapped to ipv6 */
	/* first 96 bits are 0:0:0:0:0:ffff and the last 32 bits are the ipv4 address */
	int option = 0;
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,
(void*)&option, sizeof(option))) {
		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
	}

	/* Bindt socket to local address */
	/* bind returns 0 on success*/
	printf("Binding socket to local address...\n");
	if (bind(socket_listen,
			bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
		freeaddrinfo(bind_address); // In the case where bind fails the resorses are still freed
		return 1;
	}
	/* release address memory */
	freeaddrinfo(bind_address);

	/* Start listening */
	printf("Listening...\n");
	/* 10 connections are allowed to queue up */
	if (listen(socket_listen, 10) < 0) {
		fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	/* accept any incoming connections */

	printf("Waiting for connection...\n");
	/* we have to store the clients connection info */
	/* the type will guarentee the type is large enough to hold this data */
	struct sockaddr_storage client_address;
	/* client len will differ depending on Ipv4/6*/
	socklen_t client_len = sizeof(client_address);
	/* now a tcp connection has been astablished */
	SOCKET socket_client = accept(socket_listen,
		(struct sockaddr*) &client_address, &client_len);
	if (!ISVALIDSOCKET(socket_client)) {
		fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	/* connection esablished, info */
	printf("Client is connected...");
	char address_buffer[100];

	/* client address, client address length (for ipv4 or 6), output buffer, and length, hostname output(leave so null), "", Flag specifies we dont want to see hostname of ip addresses */
	getnameinfo((struct sockaddr*)&client_address,
		client_len, address_buffer, sizeof(address_buffer), 0, 0,
		NI_NUMERICHOST);
	printf("%s\n", address_buffer);

	
	/* we now expect a https reuqest since this is setup to be a webserver */
	printf("Reading request...\n");
	char request[1024];
	int bytes_received = recv(socket_client, request, 1024, 0);
	printf("Received %d bytes.\n", bytes_received);

	/* print bytes_received number of request*/
	printf("%.*s", bytes_received, request);

	/* response */
	printf("Sending response...\n");
	const char* response =
		"HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-Type: text/plain\r\n\r\n"
		"Local time is: ";
	int bytes_sent = send(socket_client, response, strlen(response), 0);
	printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

	time_t timer;
	time(&timer);
	char buffer[26];
	// ctime_s(buffer, sizeof(buffer), &timer);
	ctime_r(&timer, buffer);
	printf("Local time is : %s", buffer);

	bytes_sent = send(socket_client, buffer, strlen(buffer), 0);
	printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(buffer));

	/* we must not close the client connection to tell the browser we have done */
	printf("Closing connection...\n");
	CLOSESOCKET(socket_client);

	/* could call accept again but instead now have to close listening socket */
	printf("Closing listening socket...\n");
	CLOSESOCKET(socket_listen);

#if defined(_WIN32)
	WSACleanup();
#endif
	printf("Finished.\n");
	return 0;
}