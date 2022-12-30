#include "mSockets/TCP/mClientTCP.h"

#include <iostream>


int main(){

    // Later date this could be made as a smart ptr if I really belive it needs to be
    mClientTCP* mcTCP = mClientTCP::getInstance();

    mcTCP->mCreate("127.0.0.1", "8080");
    mcTCP->mConnect();

    while(1){
        mcTCP->mSend("Hello");
        mcTCP->mRecv();
    }
    


    mcTCP->mClose();

    delete(mcTCP);

    return 0;
}