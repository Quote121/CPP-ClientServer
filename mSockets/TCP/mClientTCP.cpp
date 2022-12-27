#include "mClientTCP.h"

#include <iostream>

bool mClientTCP::mCreate(std::string hostName, std::string port)
{
	// Not filled in 
	if (hostName.c_str() == NULL || port.c_str() == NULL){
		return false;
	}

	// Set windows socket version
#ifdef _WIN32
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
		std::cerr << "Failed to initalize." << std::endl;
		return false;
	}
#endif

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	// TCP UDP(SOCK_DGRAM)
	hints.ai_socktype = SOCK_STREAM;
	// struct addrinfo* peer_address;
	if (getaddrinfo(hostName.c_str(), port.c_str(), &hints, &peer_address)) {
		std::cerr << "mTCP::ClientCreateTCP error.\ngetaddrinfo() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}

	printf("Remote address is: "); // TODO remove
	// Again variable arrays potentially
	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
		address_buffer, sizeof(address_buffer),
		service_buffer, sizeof(service_buffer),
		NI_NUMERICHOST);
	printf("%s %s\n", address_buffer, service_buffer); // TODO remove

	// Create socket
	printf("Creating socket...\n");
	// SOCKET socket_peer;
	socket_peer = socket(peer_address->ai_family,
		peer_address->ai_socktype, peer_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_peer)) {
		std::cerr << "mTCP::ClientCreateTCP error.\nsocket() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}
}

bool mClientTCP::mConnect()
{
	// Connect
	printf("Connecting...\n");
	if (connect(socket_peer,
		peer_address->ai_addr, peer_address->ai_addrlen)) {
		fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
		return false;
	}
	freeaddrinfo(peer_address);

	printf("Connected.\n");
    
    return true;
}



bool mClientTCP::mRecv(mClient _client)
{
	char read[4096];

	int bytes_recived = recv(socket_peer, read, 4096, 0);
	
	// TODO write this
	// Keep reading in our expaning byte array 4KiB at a time
	while (bytes_recived >= 1){
		
	}

	if (bytes_recived < 1) {

		printf("Connection closed by peer.\n");
	}

	printf("Recived (%d bytes): %.*s",
		bytes_recived, bytes_recived, read);
}

bool mClientTCP::mSend(mClient _client, std::string _msg)
{

}

bool mClientTCP::mClose()
{
    printf("Closing socket...\n");
	CLOSESOCKET(socket_peer);

#ifdef _WIN32
	WSACleanup();
#endif

	printf("Closed.\n");
    return true;
}