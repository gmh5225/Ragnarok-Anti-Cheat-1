#pragma once
#include <Windows.h>
#include <string>
#include <random>
#include <codecvt>



#define WIDESTRING_COMPARE(s1, s2) _wcsicmp(s1, s2)




/// <summary>
/// Generate random alpha-numeric string
/// </summary>
/// <param name="length">Desired length. 0 - random length from 5 to 15</param>
/// <returns>Generated string</returns>
#define GENERATE_RANDOM_WSTRING(length) \
    [](int len) { \
        static constexpr wchar_t alphabet[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefghijklmnopqrstuvwxyz1234567890"; \
        static std::random_device rd; \
        static std::uniform_int_distribution<> dist(0, _countof(alphabet) - 2); \
        std::wstring result; \
        len = (len == 0) ? (5 + (rd() % 11)) : len; \
        for (int i = 0; i < len; i++) { \
            result.push_back(alphabet[dist(rd)]); \
        } \
        return result; \
    }(length) // Pass the desired length as an argument


/// <summary>
/// Returns ANS string length.
/// </summary>
/// <param name="str">string data.</param>
/// <returns>String length.</returns>
int StringLengthA(char* str) 
{
	int length;

	for (length = 0; str[length] != '\0'; length++) {}
	return length;
}


/// <summary>
/// Converts a char* into wide.
/// </summary>
/// <param name="str">string data,</param>
/// <returns>String data as a wide string.</returns>
wchar_t* CharToWChar_T(char* str) 
{

	int length = StringLengthA(str);

	if (str == nullptr) {
		return nullptr;
	}

	wchar_t* wstr_t = (wchar_t*)malloc(sizeof(wchar_t) * length + 2);

	for (int i = 0; i < length; i++) {
		wstr_t[i] = str[i];
	}
	wstr_t[length] = '\0';
	return wstr_t;
}