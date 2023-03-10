#include "mServerTCP.h"

// Static variable definition
std::vector<std::unique_ptr<mClient>> mServerTCP::clients;
std::mutex mServerTCP::mutex_;
mServerTCP* mServerTCP::pinstance_{nullptr};

mServerTCP* mServerTCP::getInstance()
{
	// Lockguard will unlock once left scope, good for if we exit due to an error
	std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new mServerTCP();
    }
    return pinstance_;
}

bool mServerTCP::mCreate(std::string _port)
{
    // Set version for winsock2
    #if defined(_WIN32)
	// WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
        std::cerr << "Failed to initalize." << errno << std::endl;
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
	if (getaddrinfo(0, _port.c_str(), &hints, &bind_address)){
		std::cerr << "mServerTCPCreate error.\ngetaddrinfo() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}

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

	// WORKS
// if (retCode = setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&retCode, sizeof(&retCode)))

	// optCode has to be used otherwise we throw an error, dont know why. Just do it
	int optCode = 0;
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, 
				reinterpret_cast<char *>(&optCode), sizeof(optCode)))
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
	return true;
}


bool mServerTCP::mListen(int connections)
{
    /* Start listening */
    std::cout << "Listening called..." << std::endl;
	/* 10 connections are allowed to queue up */
	if (listen(socket_listen, connections) < 0) {
        std::cerr << "listen() failed. (" << GETSOCKETERRNO() << ")" << std::endl;
		return false;
	}
    return true;
	/* accept any incoming connections */
}


// TODO add a recv method where we first aquire the username of the user
bool mServerTCP::mAccept()
{

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

	std::string userName = getUserNameFromUser(socket_client);

	// Add to list of clients
	std::unique_ptr<mClient> mc = std::make_unique<mClient>(mClient(client_address, client_len, socket_client, std::time(nullptr), userName));
	addClientToList(std::move(mc));

    return true;
}


// Future method to send (DM to individual clients)
bool mServerTCP::mSend(mClient _client, std::string _msg)
{
	// Call serializer here

    send(_client.getSocketClient(), _msg.c_str(), strlen(_msg.c_str()), 0);
	return true;
}

// Send to all connected clients
bool mServerTCP::mBroadcast(std::string& _message, mClient& _client, msgType _type){

	char buffer[serializer::PACKETSIZEBYTES];
	std::memset(buffer, 0, serializer::PACKETSIZEBYTES); // Zero out buffer to avoid garbage data
	int size = serializer::serialize(buffer, _message, _client, _type);

    /* for each client */
    for(auto& c : clients){
        /* send msg */
        send(c->getSocketClient(), buffer, size, 0);
    }

    return true;
}


std::string mServerTCP::mRecv(mClient _client)
{
	// A check to check if the socket is closed would be good here

	int read_buffer_size = 1024; // for reading chuncks of data
	char* read_buffer = (char*)calloc(serializer::PACKETSIZEBYTES, sizeof(char));

	// Get packet size	
	char packet_size_bytes[4];
	recv(_client.getSocketClient(), packet_size_bytes, 4, 0);
	int packet_size = serializer::bytesToInt<uint32_t>(packet_size_bytes);
	
	std::cout << "Server recv() pakcetsize: " << packet_size << std::endl;

	// Read packet until hit specified packet size
	int bytes_recived = 0;
	while (bytes_recived < packet_size){
		// For future might have to write to temp buffer and then add to total buffer so that we can double size of char array
		// if the packet is too big
		bytes_recived += recv(_client.getSocketClient(), read_buffer, 1024, 0);
		
		// Packet is too big
		if (bytes_recived > serializer::PACKETSIZEBYTES){
			return "PACKET TOO BIG (-1)";
		}

		// Double char buffer
		if (bytes_recived >= read_buffer_size){
			read_buffer = (char*)realloc(read_buffer, read_buffer_size *= 2);
		}
		std::cout << "Bytes recieved" << bytes_recived << std::endl;
	}
	
	// Deserialize into packet struct and free read_buffer
	PACKET p = serializer::deserialize(read_buffer, packet_size); 
	free(read_buffer);


	// Construct message based on type
	return formatPacket(p);
}




bool mServerTCP::mClose()
{
    /* close all client tcp sockets and local listen socket*/
    for (auto& c : clients)
    {
        CLOSESOCKET(c->getSocketClient());
		removeClientFromList(std::move(c));
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

// Client list access methods // All should be atomic

mClient mServerTCP::getClientAtIndex(int i)
{
	return *clients.at(i);
}

void mServerTCP::addClientToList(std::unique_ptr<mClient> _client)
{
	std::lock_guard<std::mutex> lock(clientMutex);


	clients.push_back(std::move(_client));

	
}

// Might need overloading based on name and socket address etc.
// TODO NOT WORKING
void mServerTCP::removeClientFromList(std::unique_ptr<mClient> _client)
{	
	std::lock_guard<std::mutex> lock(clientMutex);

	clients.push_back(std::move(_client));
	// Use algorithms find methods
}


std::string mServerTCP::getUserNameFromUser(SOCKET socket_client){
		// get username
	std::string userNameMsg = "Please enter your username. (Max 16 characters)";
	// send user name msg
	send(socket_client, userNameMsg.c_str(), strlen(userNameMsg.c_str()), 0);

	/* loop until we have an acceptable sized username */
	
	// Max 16 chars
	char buffer[16];

	

	memset(buffer, 0, 16); // Zero out buffer
	// Read the client username into the buffer
	recv(socket_client, buffer, 16, 0);

	std::string s = buffer;

	return s;
}


std::string mServerTCP::formatPacket(PACKET& pkt){

	/////////////////////////////
	// based on packet type 
	//
	// pkt.type == msgType::CONNECT 
	// "time user has connected"
	// pkt.type == msgType::DISCONNECTED 
	// "time user has disconnected"
	// pkt.type == msgType::STANDARD 
	// "time user : msg"
	///////////////////////////////

	std::stringstream ss;

	switch(pkt.type){

		case msgType::CONNECT:
			ss << "[" << pkt.time << "]" << " user " << pkt.username << " has connected";
			break;
		case msgType::DISCONNECT:
			ss << "[" << pkt.time << "]" << " user " << pkt.username << " has disconnected";
			break;
		case msgType::STANDARD:
			ss << "[" << pkt.time << "]" << " " << pkt.username << " : " << pkt.message;
			break;
		// Fault not a valid type of packet
		default:
			ss << "\nNOT A VALID TYPE OF PACKET | mServerTCP::formatPacket()";
	}
	// Here as it might not be used
	// Can be easily removed if need be
	ss << "\n";

	return ss.str();
}