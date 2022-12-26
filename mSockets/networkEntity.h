#pragma once


#ifndef NETWORKENTITY_H
#define NETWORKENTITY_H

#include "common.h"
#include "mClient.h"
#include <string>

class NetworkEntity{

protected:
    SOCK_TYPE sockType;
    IPFamily ipFam;

public:

    NetworkEntity() = default;
    virtual ~NetworkEntity(){}

    // Sending data to a particular socket
    virtual bool mSend(mClient _client, std::string _msg);
    // receving data from a particular socket
    virtual bool mRecv(mClient _client) = 0;
    // Close all connections and call detructors for all allocated recorses
    virtual bool mClose();


    // Getters and setters
    void setSockType(SOCK_TYPE st){
        sockType = st;
    }
    SOCK_TYPE getSockType(){
        return sockType;
    }

    void setIpFamily(IPFamily ipf){
        ipFam = ipf;
    }
    IPFamily getIPFamily(){
        return ipFam;
    }


};

#endif // NETWORKENTITY_H