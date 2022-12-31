#include "mSockets/TCP/mClientTCP.h"

#include <iostream>

// Conio is for _kbhit
#ifdef _WIN32
#include <conio.h>
#endif


int main(){

    // Later date this could be made as a smart ptr if I really belive it needs to be
    mClientTCP* mcTCP = mClientTCP::getInstance();

    mcTCP->mCreate("127.0.0.1", "8080");
    mcTCP->mConnect();

    // while(1){
    //     mcTCP->mSend("Hello");
    //     mcTCP->mRecv();
    // }

    

    
    // Input loop
    while (true) {

		fd_set reads;
		FD_ZERO(&reads);
		FD_SET(mcTCP->getSocketPeer(), &reads);
#ifndef _WIN32 // non windows systems
		// 0 is the file descriptor for stdin
		// Adds console input to the program for non windows systems
		FD_SET(0, &reads); 
#endif

		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000; // 100 miliseconds in microseconds

		if (select(mcTCP->getSocketPeer() + 1, &reads, 0, 0, &timeout) < 0) {
			fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
			return 1;
		}
	
	
		if (FD_ISSET(mcTCP->getSocketPeer(), &reads)) {
			char read[4096];
			int bytes_recived = recv(mcTCP->getSocketPeer(), read, 4096, 0);
			if (bytes_recived < 1) {
				printf("Connection closed by peer.\n");
				break;
			}
			printf("Recived (%d bytes): %.*s",
				bytes_recived, bytes_recived, read);
		}
	
#ifdef _WIN32
		if (_kbhit()) {
#else
		if (FD_ISSET(0, &reads)) {
#endif
			char read[4096];
			if (!fgets(read, 4096, stdin)) break;
			printf("Sending: %s", read);
			int bytes_sent = send(mcTCP->getSocketPeer(), read, strlen(read), 0);
			printf("Sent %d bytes.\n", bytes_sent);
		}

	}




    mcTCP->mClose();

    delete(mcTCP);

    return 0;
}