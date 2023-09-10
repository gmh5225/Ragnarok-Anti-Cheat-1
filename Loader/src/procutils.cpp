#include "procutils.h"
#include "strutils.h"






FARPROC GetFunctionAddressW(HMODULE moduleHandle, const wchar_t* method)
{

    if (!moduleHandle)       
        return nullptr;
    

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)moduleHandle;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)moduleHandle + dosHeader->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)moduleHandle + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    if (importDescriptor != NULL)
    {
        while (importDescriptor->Name != 0)
        {
            const char* importedModuleName = (const char*)((DWORD_PTR)moduleHandle + importDescriptor->Name);

            PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)moduleHandle + importDescriptor->OriginalFirstThunk);

            while (thunk && thunk->u1.Function)
            {
                if (!(thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG))
                {
                    PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)moduleHandle + thunk->u1.AddressOfData);
                    char* importedFunctionName = (char*)importByName->Name;
                    wchar_t* wideImportedFunctionName = CharToWChar_T(importedFunctionName);                    

                    if (WIDESTRING_COMPARE(method, wideImportedFunctionName) == 0)
                    {                       
                        DWORD_PTR functionRVA = ntHeaders->OptionalHeader.ImageBase + (DWORD_PTR)thunk->u1.Function;     
                        delete[] wideImportedFunctionName;
                        return (FARPROC)functionRVA;
                    }   

                    delete[] wideImportedFunctionName;
                }
                thunk++;
            }
            importDescriptor++;
        }
    }

    return nullptr;
}


/// <summary>
/// Fetches the process PEB base address.
/// </summary>
/// <returns>PEB base address, otherwise nullptr.</returns>
PEB* PebBaseAddress()
{

    PROCESS_BASIC_INFORMATION pbi{};

    auto ptrNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationProcess");

    if (ptrNtQueryInformationProcess)
    {
        NTSTATUS status = ptrNtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof(pbi), nullptr);
        return (status == 0) ? pbi.PebBaseAddress : nullptr;
    }

    // ...
}


void SecureErase(void* data, size_t size) 
{
    volatile char* p = static_cast<volatile char*>(data);

    while (size--)    
        *p++ = 0; 
}