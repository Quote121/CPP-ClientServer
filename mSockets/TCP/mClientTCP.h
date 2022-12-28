#pragma once

#ifndef MCLIENTTCP_H
#define MCLIENTTCP_H

#include "../common.h"
#include "../mClient.h"
#include <iostream>

class mClientTCP{
private:
    
    SOCKET socket_peer;
    struct addrinfo* peer_address;
    

public:
    mClientTCP() = default;

    bool mCreate(std::string hostName, std::string port);

    bool mConnect();

    // TODO return packet for processing
    std::string mRecv();

    bool mSend(std::string _msg);

    bool mClose();


    // SOCKET getSocket();
    // struct addrinfo* getPeerAddress();


};


#endif // MCLIENTTCP_H