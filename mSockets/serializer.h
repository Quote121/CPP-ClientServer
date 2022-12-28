#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <cstring>
#include "mClient.h"

typedef struct PACKET{
    std::string msg;
    time_t time;
    mClient sender;
};

class serializer{
private:
    std::string msg;
    time_t time;
    mClient sender;


public:

    
    // testPacket
    // char myPacket[256] = "TestName" + {0,0,0,0,0,0,0,0} + "t";

    static void serialize();
    static PACKET deserialize();

};


#endif // SERIALIZER_H