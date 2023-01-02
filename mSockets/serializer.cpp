#include "serializer.h"


// Declaring static types
// template<typename T>
// static T bytesToInt(char* _bytes);

// template<typename T, unsigned int U>
// static std::array<std::byte, U> intToBytes(T _val);










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


PACKET serializer::deserialize(char* buffer, unsigned int size){

    // Get timestamp
    char timeStamp[sizeof(time_t)];
    std::copy(buffer, buffer + sizeof(time_t), timeStamp);
    
    int time = 0;
    // 64 bit value
    if (sizeof(time_t) == 8)
        time = bytesToInt<uint32_t>(timeStamp);
    // 32 bit value
    else time = bytesToInt<uint64_t>(timeStamp);

    char msgType;
    std::copy(buffer + sizeof(time_t), buffer + sizeof(time_t), msgType);
    // Copy over 1 byte for msgType

    // Next 16 bytes are username
    // char userName[16];
    std::string userName;
    std::copy(buffer + sizeof(time_t) + 1, buffer + sizeof(time_t) + 17, msgType);

    // The rest are the message
    std::string msg;
    std::copy(buffer + sizeof(time_t) + 17, buffer + size, msg);

    // Construct packet to return

    // TODO change msgType::CONNECT to byte value
    PACKET myPacket{size, time, msgType::CONNECT, userName, msg};
    return myPacket;
}

// Turn char* array to int
// T - typename uint16_t, uint32_t, uint64_t
template<typename T>
T serializer::bytesToInt(char* _bytes){
    // Copy and cast to unsigned char
    T num = 0;
    unsigned typeSize = sizeof(T);

    for(unsigned int i = 0; i < typeSize; i++)
    {
        num += (static_cast<T>(static_cast<unsigned char>(_bytes[i])) << 8*(typeSize-i-1));
    }
    return num;
}


// Turn int to std::byte array
// T - typename uint16_t, uint32_t, uint64_t
// U - unsigned int, size of std::array
template<typename T, unsigned int U>
std::array<std::byte, U> serializer::intToBytes(T _val)
{
    const int size = U;
    std::array<std::byte, size>bytes;

    for (int i = 0; i < size; i++){
        bytes[size-i-1] = static_cast<std::byte>((_val >> (8*i)) & 0xff);
    }
    return bytes;
}