#pragma once

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <cstring>

#include "mClient.h"

// Standard int sizes
#include <stdint.h>
#include <bit> // system endianness

#include <iostream> // debugging
#include <array>

// For array copying
#include <algorithm>






//
// We have 3 types of messages so far
// "USER : has connected "
// " [TIME] | NAME : MSG"
// "USER : has disconnected"

// TODO check
enum class msgType{
    CONNECT = static_cast<char>(0),
    DISCONNECT = static_cast<char>(1),
    STANDARD = static_cast<char>(2)
};


struct PACKET{
    uint32_t size;
    uint64_t time;
    msgType type;
    std::string username;
    std::string message;
};

class serializer{

public:


    template<typename T>
    static T bytesToInt(char* _bytes);

    template<typename T, unsigned int U>
    static std::array<std::byte, U> intToBytes(T _val);





    // Returns char* (bytes)
    // TODO later on evaluate wether switching to std::bytes is a good idea and worth it
    static char* serialize(std::string _message, mClient _client, msgType _type);
    static PACKET deserialize(char * buffer, unsigned int size);
};


#endif // SERIALIZER_H