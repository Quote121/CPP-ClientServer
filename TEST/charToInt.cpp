#include <iostream>
#include <bitset>
#include <bit> // system endianness
#include <vector>
#include <cstring>
#include <cstddef> // std::bytes
#include <array>
		

uint16_t bytesToInt16(char* data);
uint32_t bytesToInt32(char* data);
uint64_t bytesToInt64(char* data);


// std::array<std::byte, U> intToBytes(T _val);

std::array<std::byte, 2> int16ToBytes(uint16_t val);
std::array<std::byte, 4> int32ToBytes(uint32_t val);
std::array<std::byte, 8> int64ToBytes(uint64_t val);

template<typename T, unsigned int U>
std::array<std::byte, U> intToBytes(T _val);


int main(){

    // LIttle endian, 0x02 is the biggest
    char msg[4] = {0x00, 0x00, 0x00, 0x02};
    
    int num = bytesToInt32(msg);
    
    std::cout << "Number: " << num << std::endl;
    std::bitset<32> x(num);
    std::cout << "Bin\n" << x;

    std::array<std::byte, 4> b = intToBytes<std::uint32_t, 4>(num);
    char* c_str = reinterpret_cast<char*>(b.data());
    
    int num2 = bytesToInt32(c_str);

    std::cout << "TEST: " << num2 << std::endl;

    return 0;
}


uint16_t bytesToInt16(char* data){

    uint16_t num;

    // Determine system endianess
    if (std::endian::native == std::endian::little){
        std::cout << "LITTLE ENDIAN" << std::endl;
        num = (data[0] << 8) | data[1];
    }
    else if (std::endian::native == std::endian::big){
        std::cout << "BIG ENDIAN" << std::endl;
        num = (data[1] << 8) | data[0];
    }
    else{
        std::cout << "UNKNOWN STD::ENDIAN::NATIVE" << std::endl;
        num = 0;
    }

    std::cout << "Number: " << num << std::endl;
    
    return num;
}


uint32_t bytesToInt32(char* data){

    uint32_t num;

    // Determine system endianess
    if (std::endian::native == std::endian::little){
        std::cout << "LITTLE ENDIAN" << std::endl;
        num = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    }
    else if (std::endian::native == std::endian::big){
        std::cout << "BIG ENDIAN" << std::endl;
        num = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    }
    else{
        std::cout << "UNKNOWN STD::ENDIAN::NATIVE" << std::endl;
        num = 0;
    }

    std::cout << "Number: " << num << std::endl;
    return num;
}

// uint64_t bytesToInt64(char* data){

//     uint64_t num;

//     // Determine system endianess
//     if (std::endian::native == std::endian::little){
//         std::cout << "LITTLE ENDIAN" << std::endl;
//         num = (data[0] << 56) | (data[1] << 48) | (data[2] << 40) | (data[3] << 32) | (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
//     }
//     else if (std::endian::native == std::endian::big){
//         std::cout << "BIG ENDIAN" << std::endl;
//         num = (data[7] << 56) | (data[6] << 48) | (data[5] << 40) | (data[4] << 32) | (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
        
//     }
//     else{
//         std::cout << "UNKNOWN STD::ENDIAN::NATIVE" << std::endl;
//         num = 0;
//     }
//     std::cout << "Number: " << num << std::endl;
//     return num;
// }


// Template can return short, int, long
template<typename T, unsigned int U>
std::array<std::byte, U> intToBytes(T _val)
{
    const int size = U;
    std::array<std::byte, size>bytes;
    std::byte val = static_cast<std::byte>(_val);
    std::byte ff{0xff};

    for (int i = 0; i < size; i++){
        bytes[size-i-1] = (val >> (8*i)) & ff;
    }
    return bytes;
}


std::array<std::byte, 2> int16ToBytes(uint16_t _val)
{
    const int size = 2;
    std::array<std::byte, size>bytes;
    std::byte val = static_cast<std::byte>(_val);
    std::byte ff{0xff};

    for (int i = 0; i < size; i++){
        bytes[size-i-1] = (val >> (8*i)) & ff;
    }
    return bytes;
}


std::array<std::byte, 4> int32ToBytes(uint32_t _val)
{
    std::array<std::byte, 4>bytes;
    
    std::byte val = static_cast<std::byte>(_val);
    
    // const int size = 4;
    // char bytes[size+1];
    // for (int i = 0; i < size; i++){
    //     bytes[size-i-1] = (val >> (8*i)) & 0xff;
    // }
    // bytes[size] = '\0';
    // return bytes;

    std::byte ff{0xff};

    std::byte b0 = val & ff;
    std::byte b1 = (val >> 8) & ff;
    std::byte b2 = (val >> 16) & ff;
    std::byte b3 = (val >> 24) & ff;

    // There are no null terminating characters as not string
    
    bytes[0] = b3;
    bytes[1] = b2;
    bytes[2] = b1;
    bytes[3] = b0;

    return bytes;
}

// std::array<std::byte, 4> int32ToBytes(uint32_t _val)
// {
//     std::array<std::byte, 4>bytes;
    
//     std::byte val = static_cast<std::byte>(_val);
    
//     // const int size = 4;
//     // char bytes[size+1];
//     // for (int i = 0; i < size; i++){
//     //     bytes[size-i-1] = (val >> (8*i)) & 0xff;
//     // }
//     // bytes[size] = '\0';
//     // return bytes;

//     std::byte ff{0xff};

//     std::byte b0 = val & ff;
//     std::byte b1 = (val >> 8) & ff;
//     std::byte b2 = (val >> 16) & ff;
//     std::byte b3 = (val >> 24) & ff;

//     // There are no null terminating characters as not string
    
//     bytes[0] = b3;
//     bytes[1] = b2;
//     bytes[2] = b1;
//     bytes[3] = b0;

//     return bytes;
// }





std::array<std::byte, 8> int64ToBytes(uint64_t _val)
{
    const int size = 8;
    std::array<std::byte, size>bytes;
    std::byte val = static_cast<std::byte>(_val);
    std::byte ff{0xff};

    for (int i = 0; i < size; i++){
        bytes[size-i-1] = (val >> (8*i)) & ff;
    }
    return bytes;

    // uint8_t b0 = val & 0xff;
    // uint8_t b1 = (val >> 8) & 0xff;
    // uint8_t b2 = (val >> 16) & 0xff;
    // uint8_t b3 = (val >> 24) & 0xff;
    // uint8_t b4 = (val >> 32) & 0xff;
    // uint8_t b5 = (val >> 40) & 0xff;
    // uint8_t b6 = (val >> 48) & 0xff;
    // uint8_t b7 = (val >> 56) & 0xff;


    // // There are no null terminating characters as not string

    // bytes[0] = b3;
    // bytes[1] = b2;
    // bytes[2] = b1;
    // bytes[3] = b0;
    // bytes[4] = b0;
    // bytes[5] = b0;
    // bytes[6] = b0;
    // bytes[7] = b0;

    // return bytes;
}