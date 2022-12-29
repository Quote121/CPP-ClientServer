#include "mSockets/TCP/mClientTCP.h"

#include <iostream>



int main(){

    mClientTCP* mcTCP = new mClientTCP();

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