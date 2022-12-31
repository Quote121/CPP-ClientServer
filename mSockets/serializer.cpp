#include "serializer.h"


// TODO Maybe Add md5 checksum at a later day to verify integrity
char* serializer::serialize(std::string _message, mClient _client, msgType _type){

    uint32_t size;
    uint64_t time;

    msgType type;

    std::string username;
    std::string message;


    // Determine the size of the packet
    type = _type;
    username = _client.getAlias();
    message = _message;
    // Get time stamp
    std::time_t result = std::time(nullptr);
    time = result;

    size = sizeof(type) + sizeof(username.c_str()) + sizeof(message.c_str()) + sizeof(time);

    // Dont belive that std::byte has any advantages
    char bytes[4096];
    std::memset(bytes, 0, 4096); // Zero out buffer
    
    std::memcpy(&bytes[0], &size, sizeof(size)); // Size of packet
    std::memcpy(&bytes[0+sizeof(size)], &time, sizeof(time)); // Time
    std::memcpy(&bytes[0+sizeof(size)+sizeof(time)], &type, sizeof(type)); // Type
    std::memcpy(&bytes[0+sizeof(size)+sizeof(time)+sizeof(type)], username.c_str(), sizeof(username.c_str())); // Username
    std::memcpy(&bytes[0+sizeof(size)+sizeof(time)+sizeof(type)+sizeof(username)], message.c_str(), sizeof(message.c_str())); // Message


    
    std::cout << "PACKET PRINT OUT" << std::endl;
    std::cout << "" << std::endl;

    return bytes;
    //////////////////////////////////
    ///
    /// Packet structure
    ///
    /// first 4 bytes (unsigned int size) Size of the packet data excluding the size
    /// Next 4/8 bytes is the time stamp
    /// since the typedef is long it could be different for different systems and so thats why the size parameter is at the beginning
    /// next 16 bytes are the username of the client
    /// the remaning size is the message from the user
    ///
    ///////////////////////////////////
    // std::memcpy(bytes, testStr.data(), testStr.length());

}


PACKET serializer::deserialize(char* buffer){
    // The inputted char* buffer will not include the size of the packet at the beginning
    // as this is used to originally recieve the data

    // Determine system endianess
    if (std::endian::native == std::endian::little){
        std::cout << "LITTLE ENDIAN" << std::endl;
    }
    else if (std::endian::native == std::endian::big){
        std::cout << "BIG ENDIAN" << std::endl;
    }
    else{
        std::cout << "UNKNOWN STD::ENDIAN::NATIVE" << std::endl;
    }
    

    std::string sbuffer = buffer;
    // For each char
    for (char& b : sbuffer){
        std::cout << static_cast<int>(b) << " ";
    }
    

    PACKET myPacket;
    return myPacket;

}