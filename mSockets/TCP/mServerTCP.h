#pragma once

#ifndef MSERVERTCP_H
#define MSERVERTCP_H

#include "../common.h" // Common network includes
#include "../mClient.h" // Client class

#include <vector>
#include <string>
#include <iostream>
#include <memory>


class mServerTCP{
private:

    // For tcp socket creation,
    // wsadata only used for win32
    WSADATA d;
    SOCKET socket_listen;

    static std::vector<std::unique_ptr<mClient>> clients;
public:
    

    mServerTCP() = default;

    // Create the tcp socket and bind to port
    bool mCreate(std::string _port);

    // Listen for incomming connections
    bool mListen(int connection);

    // On connection, accept and add the client to our clients vector
    bool mAccept();

    // Send message to specified client
    bool mSend(mClient _client, std::string _msg);

    // Recieve from specific client
    std::string mRecv(mClient _client);

    // Send message to all clients
    bool mBroadcast(std::string _msg);

    // Close all clients and server
    bool mClose();

    static mClient getClientAtIndex(int i);

};


#endif // MSERVERTCP_H