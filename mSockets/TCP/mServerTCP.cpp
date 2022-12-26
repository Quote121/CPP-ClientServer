#include "mServerTCP.h"

mServerTCP::mServerTCP() = default;

bool mServerTCP::mCreate(std::string _port)
{
    // Set version for winsock2
    #if defined(_WIN32)
	// WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
        std::cerr << "Failed to initalize." << std::endl;
		return false;
	}
    #endif

    // TODO ALTER TO C++
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
	getaddrinfo(0, _port.c_str(), &hints, &bind_address);

	printf("Creating socket...\n");
	// SOCKET socket_listen;
	socket_listen = socket(bind_address->ai_family,
		bind_address->ai_socktype, bind_address->ai_protocol);

	/* check if the call to socket() was sucessful */
	if (!ISVALIDSOCKET(socket_listen)) {
        std::cerr << "socket() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}

	/* implementation of duel stack ipv4 and ipv6 */
	/* when an ipv4 connection connects the ipv4 is remapped to ipv6 */
	/* first 96 bits are 0:0:0:0:0:ffff and the last 32 bits are the ipv4 address */
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, NULL, NULL)) 
    {
        std::cerr << "setsockopt() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
	}

	/* Bindt socket to local address */
	/* bind returns 0 on success*/
	printf("Binding socket to local address...\n");
	if (bind(socket_listen,
			bind_address->ai_addr, bind_address->ai_addrlen)) {
        std::cerr << "bind() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		freeaddrinfo(bind_address); // In the case where bind fails the resorses are still freed
		return false;
	}
	/* release address memory */
	freeaddrinfo(bind_address);
}

bool mServerTCP::mListen(int connections)
{
    /* Start listening */
    std::cout << "Listening called..." << std::endl;
	/* 10 connections are allowed to queue up */
	if (listen(socket_listen, 10) < 0) {
        std::cerr << "listen() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}
    return true;
	/* accept any incoming connections */
}

bool mServerTCP::mAccept()
{

    // TODO Create mew client object as unique ptr
    // add the client to the clients vector 

    std::cout << "acceptTCP() called" << std::endl;
	/* we have to store the clients connection info */
	/* the type will guarentee the type is large enough to hold this data */
	struct sockaddr_storage client_address;
	/* client len will differ depending on Ipv4/6*/
	socklen_t client_len = sizeof(client_address);
	/* now a tcp connection has been astablished */
    /* potentially blocking*/
	SOCKET socket_client = accept(socket_listen,
		(struct sockaddr*) &client_address, &client_len);
	if (!ISVALIDSOCKET(socket_client)) {
        std::cerr << "accept() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}

	/* connection esablished, info */
	printf("Client is connected...");
	char address_buffer[100];

	/* client address, client address length (for ipv4 or 6), output buffer, and length, hostname output(leave so null), "", Flag specifies we dont want to see hostname of ip addresses */
	getnameinfo((struct sockaddr*)&client_address,
		client_len, address_buffer, sizeof(address_buffer), 0, 0,
		NI_NUMERICHOST);
	printf("%s\n", address_buffer);

    return true;
}



bool mServerTCP::mSend(mClient _client, std::string _msg)
{
    send(_client.getSocketClient(), _msg.c_str(), strlen(_msg.c_str()), 0);
}


bool mServerTCP::mBroadcast(std::string _msg){

    /* for each client */
    for(auto c : clients){
        /* send msg */
        send(c.getSocketClient(), _msg.c_str(), strlen(_msg.c_str()), 0);
    }

    return true;
}


bool mServerTCP::mRecv(mClient _client)
{
	/* we now expect a https reuqest since this is setup to be a webserver */
	printf("Reading request...\n");
	/* Can make dynamic array to read into */
	/* increase size etc */
	char request[1024];
	int bytes_received = recv(_client.getSocketClient(), request, 1024, 0);
	printf("Received %d bytes.\n", bytes_received);

	/* print bytes_received number of request*/
	printf("%.*s", bytes_received, request);

    return true;
}

bool mServerTCP::mClose()
{
    /* close all client tcp sockets and local listen socket*/
    for (auto c : clients)
    {
        CLOSESOCKET(c.getSocketClient());
    }
    /* close listening socket */
    CLOSESOCKET(socket_listen);

    /* windows winsock cleanup*/
#if defined(_WIN32)
	WSACleanup();
#endif

    /* call destructors */

    return true;
}
