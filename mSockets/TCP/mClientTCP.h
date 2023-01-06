#pragma once

#ifndef MCLIENTTCP_H
#define MCLIENTTCP_H

#include "../common.h"
#include "../mClient.h"
#include "../serializer.h"
#include <iostream>
#include <mutex>

class mClientTCP{
private:
    // For threadsafe singleton
    static mClientTCP * pinstance_;
    static std::mutex mutex_;
    
    // Socket variables
    SOCKET socket_peer;
    struct addrinfo* peer_address;
    
    // To stop initalization, copy and assignment
    mClientTCP() = default;
    mClientTCP(const mClientTCP&) = delete;
    mClientTCP& operator=(const mClientTCP&) = delete;

public:

    // Way we create an object, only one can be created
    static mClientTCP* getInstance();

    // Create the socket
    bool mCreate(std::string hostName, std::string port);

    // Connect to the socket specified by hostname and port in create
    bool mConnect();

    // TODO return packet for processing
    std::string mRecv();

    // Send message to the server
    // Here client is always the same and so maybe for future we get rid of this field as its more for server
    bool mSend(std::string& _message, mClient& _client, msgType _type);

    // Close down the curretn socket
    bool mClose();


    ///////////////////////////
    ///
    /// Getters and setters
    ///
    //////////////////////////
    SOCKET getSocketPeer(){
        return socket_peer;
    }

    struct addrinfo* getPeerAddress(){
        return peer_address;
    }

};


#endif // MCLIENTTCP_H