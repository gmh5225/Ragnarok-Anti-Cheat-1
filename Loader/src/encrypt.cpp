#include "encrypt.h"
#include "strutils.h"





char* EncryptStringW(const wchar_t* message, std::wstring& enckey, int& bits)
{

    CryptoPP::GOST::Encryption cipher;
    bits = RANDOM_INT();
    CryptoPP::SecByteBlock key(bits);

    enckey = GENERATE_RANDOM_WSTRING(bits);
    std::string saucyBytes(enckey.begin(), enckey.end());

    memcpy(key.data(), saucyBytes.c_str(), saucyBytes.size());

    cipher.SetKey(key, key.size());

    size_t messageSize = wcslen(message) * sizeof(wchar_t);
    unsigned char encrypted_message[messageSize];

    cipher.ProcessBlock((const unsigned char*)(message), encrypted_message);

    char* encryptedCString = (char*)(encrypted_message);
    encryptedCString[sizeof(encrypted_message) - 1] = '\0';
    
    return encryptedCString;
}