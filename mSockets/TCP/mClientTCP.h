#pragma once

#ifndef MCLIENTTCP_H
#define MCLIENTTCP_H

#include "../common.h"
#include "../networkEntity.h"
#include "../mClient.h"

class mClientTCP : NetworkEntity{
private:
    
    SOCKET socket_peer;
    struct addrinfo* peer_address;

public:

    bool mCreate(std::string hostName, std::string port);

    bool mConnect();

    bool mRecv(mClient _client) override;

    bool mSend(mClient _client, std::string _msg) override;

    bool mClose() override;

};


#endif // MCLIENTTCP_H