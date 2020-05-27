#pragma once




#include <stdint.h>
#define SHA1HashSize 20
enum
{
    shaSuccess = 0,
    shaNull,         /* Null pointer parameter */
    shaInputTooLong, /* input data too long */
    shaStateError    /* called Input after Result */
};
class  SHA2
{
    uint32_t Intermediate_Hash[SHA1HashSize / 4]; /* Message Digest */
    uint32_t Length_Low; /* Message length in bits */
    uint32_t Length_High; /* Message length in bits */
    int_least16_t Message_Block_Index;
    uint8_t Message_Block[64]; /* 512-bit message blocks */
    int Computed; /* Is the digest computed? */
    int Corrupted; /* Is the message digest corrupted? */
public:
    SHA2();
    ~SHA2();
    void Reset();
    int Input(const uint8_t*, unsigned int);
    int Result(uint8_t Message_Digest[SHA1HashSize]);
    void PadMessage();
    void ProcessMessageBlock();
    int  TransformToHASH(const char* input, uint8_t output[20], unsigned size);
};

