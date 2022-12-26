#ifndef mTCP_H
#define mTCP_H


#include "common.h" // Common socket libs
#include "mClient.h" // Clients that connect to the serve
#include <vector>
#include <iostream>
#include <string>

// Create my own client struct or we can do a class


class mTCP{
private:
    // For creating tcp socket
    // WSADATA only used for windows
    WSADATA d;
    SOCKET socket_listen;
    
public:
    // List of connected client
    std::vector<mClient> clients;
    
    // Defualt constructor
    mTCP();

    // Will create a winsock tcp or posix tcp socket
    // Return bool on how it went
    // Will create and bind to port
    // Will always remap ipv4 to 6
    bool mCreateTCP();

    // Server listen
    // connections are the number of connections we can accept
    bool mListenTCP(int connections);


    // Here we add client to our list
    // Server accept
    bool mAcceptTCP();

    // Recieve from certain socket/client
    bool mRecv(mClient _client);

    // Broadcast to all clients
    bool mBroadcast(std::string _msg);
    


    // Server close
    bool mCloseTCP();

    



    // Getters and setters



};

#endif // mTCP_H
