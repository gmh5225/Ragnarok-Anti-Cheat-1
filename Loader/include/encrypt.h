#pragma once
#include <windows.h>
#include <iostream>
#include "cryptopp/gost.h"
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>




#define RANDOM_INT() (std::rand() % 3 == 0 ? 16 : (std::rand() % 2 == 0 ? 24 : 32))


char* EncryptStringW(const wchar_t* message, std::wstring& enckey, int& bits);