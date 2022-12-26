#pragma once

#ifndef MSERVERTCP_H
#define MSERVERTCP_H

#include "../common.h" // Common network includes
#include "../networkEntity.h" // Network abstract class
#include "../mClient.h" // Client class

#include <vector>
#include <string>
#include <iostream>

class mServerTCP : NetworkEntity{
private:

    // For tcp socket creation,
    // wsadata only used for win32
    WSADATA d;
    SOCKET socket_listen;

    std::vector<mClient> clients;

public:
    mServerTCP();

    // Create the tcp socket and bind to port
    bool mCreate(std::string _port);

    // Listen for incomming connections
    bool mListen(int connection);

    // On connection, accept and add the client to our clients vector
    bool mAccept();

    // Send message to specified client
    bool mSend(mClient _client, std::string _msg) override;

    // Recieve from specific client
    bool mRecv(mClient _client) override;

    // Send message to all clients
    bool mBroadcast(std::string _msg);

    // Close all clients and server
    bool mClose() override;

};


#endif // MSERVERTCP_H