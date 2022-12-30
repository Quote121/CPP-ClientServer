#pragma once

#ifndef MSERVERTCP_H
#define MSERVERTCP_H

#include "../common.h" // Common network includes
#include "../mClient.h" // Client class

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <mutex>

class mServerTCP{
private:
    // For threadsafe singleton
    static mServerTCP * pinstance_;
    static std::mutex mutex_;

    // For tcp socket creation,
    // wsadata only used for win32
    WSADATA d;
    SOCKET socket_listen;

    // To stop initalization
    mServerTCP() = default;

    // To stop copy
    mServerTCP(const mServerTCP&) = delete;

    // To stop assignment
    // = delete to not allow equal
    mServerTCP& operator=(const mServerTCP&) = delete;

    static std::vector<std::unique_ptr<mClient>> clients;
public:
    // For thread access to clients
    std::mutex clientMutex;

    // Way we create an object, only one can be created
    static mServerTCP* getInstance();


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

    void addClientToList(std::unique_ptr<mClient> _client);

    void removeClientFromList(std::unique_ptr<mClient> _client);

};




#endif // MSERVERTCP_H