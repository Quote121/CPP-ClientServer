#pragma once

#ifndef MCLIENTTCP_H
#define MCLIENTTCP_H

#include "../common.h"
#include "../mClient.h"
#include <iostream>
#include <mutex>

class mClientTCP{
private:
    // For threadsafe singleton
    static mClientTCP * pinstance_;
    static std::mutex mutex_;
    
    SOCKET socket_peer;
    struct addrinfo* peer_address;
    
    // To stop initalization, copy and assignment
    mClientTCP() = default;
    mClientTCP(const mClientTCP&) = delete;
    mClientTCP& operator=(const mClientTCP&) = delete;

public:

    // Way we create an object, only one can be created
    static mClientTCP* getInstance();


    bool mCreate(std::string hostName, std::string port);

    bool mConnect();

    // TODO return packet for processing
    std::string mRecv();

    bool mSend(std::string _msg);

    bool mClose();

};


#endif // MCLIENTTCP_H