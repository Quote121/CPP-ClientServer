#include "serializer.h"

#include <array>

#include <cstddef>
#include <memory>
// Declaring static types
// template<typename T>
// T bytesToInt(char* _bytes);

// template<typename T, unsigned int U>
// std::array<std::byte, U> intToBytes(T _val);


template uint16_t serializer::bytesToInt(char* _bytes);
template uint32_t serializer::bytesToInt(char* _bytes);
template uint64_t serializer::bytesToInt(char* _bytes);

// static definition


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


// TODO Maybe Add md5 checksum at a later day to verify integrity
// returns the size of a packet in bytes excluding the size indicator at beginning of packet
// 
// returns uint32_t - size of packet in bytes
//
// char* bytes - the byte buffer to write to
// std::string& _message - message written by user (empty if disconnect or connect)
// mClinet& _client - client that send the message and so we can get the username from this
// msgType _type - enum to show what type of msg we are sending
uint32_t serializer::serialize(char* bytes, std::string& _message, mClient& _client, msgType _type){

    uint32_t size;
    uint64_t time;

    char type;

    std::string username;
    std::string message;

    // Get time stamp
    time = std::time(nullptr);
    type = static_cast<int>(_type);
    username = _client.getAlias();
    message = _message;

    // Size of packet does not include the size of bytes
    //     time           msg type       username            message
    size = sizeof(time) + sizeof(type) + USERNAMESIZEBYTES + message.size();

    // char bytes[PACKETSIZEBYTES];
    std::memset(bytes, 0, PACKETSIZEBYTES);

    // Convert int to bytes
    std::array<std::byte, 4> bsize = intToBytes<uint32_t, 4>(size);
    std::array<std::byte, 8> btime = intToBytes<uint64_t, 8>(time);

    // copy memory to buffer
    std::memcpy(bytes, &bsize, sizeof(size)); // Size of packet
    std::memcpy(bytes+sizeof(size), &btime, sizeof(time)); // Time
    std::memcpy(bytes+sizeof(size)+sizeof(time), &type, sizeof(type)); // Type
    std::memcpy(bytes+sizeof(size)+sizeof(time)+sizeof(type), username.data(), username.size()); // Username
    std::memcpy(bytes+sizeof(size)+sizeof(time)+sizeof(type)+USERNAMESIZEBYTES, message.data(), message.size()); // Message

    // Debug print bytes
    // std::cout << std::hex;
    // for (int i = 0; i < size+4; i++){
    //     std::cout << +std::to_integer<int>(std::byte(bytes[i])) << " ";
    // }
    // std::cout << std::dec << std::endl;

    return size;

}


// Deserialize a stream of bytes into packet struct
// char* buffer - input buffer to read from
// unsigned int size - size of the packet
PACKET serializer::deserialize(char* buffer, unsigned int size){

    // debug
    // std::cout << std::hex;
    // for (int i = 0; i < size; i++){
    //     std::cout << +std::to_integer<int>(std::byte(buffer[i])) << " ";
    // }
    // std::cout << std::dec << std::endl;
    
    // Get timestamp
    char timeStamp[sizeof(uint64_t)];
    std::copy(buffer, buffer + sizeof(uint64_t), timeStamp);
    uint64_t time = bytesToInt<uint64_t>(timeStamp);;

    // Get msgType
    char bmsgType;
    std::copy(buffer + sizeof(uint64_t), buffer + sizeof(uint64_t) + sizeof(char), &bmsgType);
    
    // Get username
    std::string userName;
    userName.assign(buffer + sizeof(uint64_t) + sizeof(char), buffer + sizeof(uint64_t) + sizeof(char) + USERNAMESIZEBYTES);

    // The rest are the message
    // Read to end of buffer specified by size
    std::string msg;
    msg.assign(buffer + sizeof(uint64_t) + sizeof(char) + USERNAMESIZEBYTES, buffer + size);

    // Construct packet to return
    PACKET myPacket{size, time, static_cast<msgType>(bmsgType), userName, msg};
    return myPacket;
}

// Turn char* array to int
// T - typename uint16_t, uint32_t, uint64_t
template<typename T>
T serializer::bytesToInt(char* _bytes){
    // Copy and cast to unsigned char
    T num = 0;
    unsigned int typeSize = sizeof(T);

    for(unsigned int i = 0; i < typeSize; i++)
    {
        num += (static_cast<T>(static_cast<unsigned char>(_bytes[i])) << 8*(typeSize-i-1));
    }
    return num;
}

// This can be sped up by passing the array via reference
// Speed increase would be small as we stop the copy
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