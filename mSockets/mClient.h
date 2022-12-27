// Class structor for a connected client
#ifndef MCLIENT_H
#define MCLINET_H

#include "common.h"
#include <ctime>

class mClient{
private:
    struct sockaddr_storage client_address;
    socklen_t client_len; // change, will be set
    IPFamily ip_family; // AF_INET, AF_INET6
    SOCKET socket_client;
    std::time_t connectionTime; // Time when the client connected
    bool connected; // Flag might not be needed

public:
    mClient();
    // Other initalise list, move to .cpp
    mClient(struct sockaddr_storage _client_address, socklen_t _client_len, SOCKET _socket_client, int _connectionTime);

    // Getters and setters

    struct sockaddr_storage getClient_address();
    void setClient_address(struct sockaddr_storage &_client_address);

    socklen_t getClientLen();
    void setClientLen(socklen_t _client_len);

    IPFamily getIpFamily();
    void setIpFamily(IPFamily _ip_family);

    SOCKET getSocketClient();
    void setSocketClient(SOCKET _socket_client);

    int getConnectionTime();
    void setConnectionTime(int _connectionTime);

};

#endif // MCLIENT_H