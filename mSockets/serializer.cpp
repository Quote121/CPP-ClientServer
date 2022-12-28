#include "serializer.h"

// Maybe Add md5 checksum at a later day to verify integrity
void serializer::serialize(){
    std::string testStr = "Rodger";
    time_t time;
    std::string msg = "Hello this is a test message";

    std::byte bytes[256];
    std::memset(bytes, 0, 256); // Zero out buffer
    
    char* myString = reinterpret_cast<char*>(&bytes);
    
    char* endOfData = "\r\r\n\r";


    // std::memcpy(bytes, testStr.data(), testStr.length());


    // Check that the string sizes are good
    if ((testStr.length() <= 8) && msg.length() <= 232){

    }

    std::memcpy( &bytes[0], &testStr[0], 8); // Write the 8 bytes of the username
    std::memcpy( &bytes[0+sizeof(testStr.length()*sizeof(char))], &time, sizeof(time_t));
    std::memcpy( &bytes[0+sizeof(testStr.length()*sizeof(char))+sizeof(time_t)], &msg[0], msg.length() * sizeof(char));
    
}

PACKET serializer::deserialize(){



}