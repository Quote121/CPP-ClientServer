#include "mClientTCP.h"


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
	return true;
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


// Recieve packet struct
// From the server
std::string mClientTCP::mRecv()
{
	// C like for speed
	
	int read_buffer_size = 1024;
	char* read_buffer = (char*)calloc(read_buffer_size, sizeof(char));

	int bytes_recived = 0;
	do {
		// Resize the read in buffer
		if (bytes_recived >= read_buffer_size){
			// Double input buffer size
			read_buffer = (char*)realloc(read_buffer, read_buffer_size *= 2);
		}
	// Keep reading while there is data
	} while ((bytes_recived = recv(socket_peer, read_buffer, 1024, 0)) > 0);

	// TODO we need to pass the sender of the message via the packet to the user
	// I.E we need a serializer and deserializer

	// Call decerializer and then return the packet struct to the caller

	std::string msg = read_buffer;
	free(read_buffer);

	return msg;
}

bool mClientTCP::mSend(std::string _msg)
{
	// Send to socket peer (server)
	send(socket_peer, _msg.c_str(), strlen(_msg.c_str()), 0);
	return true;
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

// // Getters and setters
// SOCKET mClientTCP::getSocket(){
// 	return socket_peer;
// }
// struct addrinfo* mClientTCP::getPeerAddress()
// {
// 	return peer_address;
// }