#include "mSockets/TCP/mServerTCP.h"

#include <iostream>
#include <thread>

// Now we need a method for dealing with connections


int main(){

    mServerTCP* msTCP = new mServerTCP();

    msTCP->mCreate("8080");
    msTCP->mListen(10);
    
    // Accepts one person
    msTCP->mAccept();

    // Recieve from 1st client
    std::string recvMsg = msTCP->mRecv(mServerTCP::getClientAtIndex(0));
    
    msTCP->mBroadcast(recvMsg);

    msTCP->mClose();
    
    return 0;
}