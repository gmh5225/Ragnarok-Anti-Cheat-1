#include "ProcessDetection.h"




int ProcessDetection::CheatEngineStringsFoundInProcess(DWORD processId)
{
    
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

    if (process == INVALID_HANDLE_VALUE)
    {               
        return FailedProcHandle;
    }

    process = (HANDLE)EncodePointer(process);

    SYSTEM_INFO systeminfo = {};
    GetSystemInfo(&systeminfo);
    LPVOID base = systeminfo.lpMinimumApplicationAddress;
    LPVOID max = systeminfo.lpMaximumApplicationAddress;
    size_t found;
    const int buffsize = 8096;
    char buffer[buffsize];

    HMODULE kernel32 = LoadLibraryExA(Kernel32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (kernel32 == INVALID_HANDLE_VALUE || nullptr)
    {
        process = (HANDLE)DecodePointer(process);
        CloseHandle(process);
        return FailedLoadLib;
    }

    DynReadProcessMemory ptrReadProcessMemory = (DynReadProcessMemory)GetProcAddress(kernel32, ReadProcessMemoryObf().c_str());

    if (ptrReadProcessMemory == nullptr)
    {
        FreeLibrary(kernel32);
        process = (HANDLE)DecodePointer(process);
        CloseHandle(process);
        return FailedLoadLib;
    }  

    process = (HANDLE)DecodePointer(process);

    while (base < max)
    {
        SIZE_T bytesRead;

        if (ptrReadProcessMemory(process, base, buffer, buffsize, &bytesRead))
        {
            std::string memoryContent(buffer, buffer + bytesRead);
           
            for (const auto& currentString : CheatEngineString)
            {
                found = memoryContent.find_first_of(currentString);

                if (found != std::string::npos)
                {                                      
                    CloseHandle(process);
                    FreeLibrary(kernel32);
                    systeminfo = {};
                    ptrReadProcessMemory = nullptr;
                    return true;
                }
            }                      
            base = (LPVOID)((DWORD)base + bytesRead);
        }
        else
        {
            base = (LPVOID)((DWORD)base + buffsize);
        }
    }

    CloseHandle(process);
    FreeLibrary(kernel32);
    systeminfo = {};
    ptrReadProcessMemory = nullptr;
    return false;
}


void ProcessDetection::DetectSeDebugPrivilegeToken()
{

    DWORD processes[1024], cbNeeded, cProcesses; 
    Dyn32EnumProcesses ptrK32EnumProcesses = nullptr;
    DynGetTokenInformation ptrGetTokenInformation = nullptr;
    DynOpenProcessToken ptrOpenProcessToken = nullptr;
    auto _LoadLibraryExA = LI_FN(LoadLibraryExA).in(LI_MODULE("kernel32.dll").cached());
    auto _OpenProcess = LI_FN(OpenProcess).in(LI_MODULE("kernel32.dll").cached());
    auto _LookupPrivilegeValueW = LI_FN(LookupPrivilegeValueW).in(LI_MODULE("Advapi32.dll").cached());


    HMODULE kernel32 = _LoadLibraryExA(Kernel32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (kernel32 == INVALID_HANDLE_VALUE) { return; }

    HMODULE advapi32 = _LoadLibraryExA(Advapi32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);
    advapi32 == nullptr ? (LI_FN(FreeLibrary)(kernel32), ptrK32EnumProcesses = nullptr) : NULL;

    FARPROC ptrGetProcAddress = FindGetProcAddress();

    ptrK32EnumProcesses = (Dyn32EnumProcesses)((FARPROC(*)(HMODULE, LPCSTR))ptrGetProcAddress)(kernel32, K32EnumProcessesObf().c_str());
    ptrK32EnumProcesses == nullptr ? (LI_FN(FreeLibrary)(kernel32), LI_FN(FreeLibrary)(advapi32), void()) : void();

    if (ptrK32EnumProcesses(processes, sizeof(processes), &cbNeeded))
    {      
        ptrOpenProcessToken = (DynOpenProcessToken)((FARPROC(*)(HMODULE, LPCSTR))ptrGetProcAddress)(advapi32, OpenProcessTokenObf().c_str());
        ptrOpenProcessToken == nullptr ? (LI_FN(FreeLibrary)(kernel32), LI_FN(FreeLibrary)(advapi32), ptrK32EnumProcesses = nullptr, void()) : void();

        ptrGetTokenInformation = (DynGetTokenInformation)((FARPROC(*)(HMODULE, LPCSTR))ptrGetProcAddress)(advapi32, GetTokenInformationObf().c_str());
        ptrGetTokenInformation == nullptr ? (LI_FN(FreeLibrary)(kernel32), LI_FN(FreeLibrary)(advapi32), ptrK32EnumProcesses = nullptr, ptrOpenProcessToken = nullptr, void()) : void();

        cProcesses = cbNeeded / sizeof(DWORD);

        for (unsigned int i = 0; i < cProcesses; i++)
        {
            if (processes[i] != 0)
            {
                HANDLE targetprocess = _OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);

                if (targetprocess == INVALID_HANDLE_VALUE) { continue; }

                HANDLE ptoken = NULL;             

                if (ptrOpenProcessToken(targetprocess, TOKEN_QUERY, &ptoken))
                {                  
                    DWORD buf = NULL;                                      

                    ptrGetTokenInformation(ptoken, TokenPrivileges, NULL, 0, &buf);    //  Cold call

                    std::vector<BYTE> buffer(buf);

                    PTOKEN_PRIVILEGES privs = (PTOKEN_PRIVILEGES)buffer.data();
                    LUID tvalue;

                    if (ptrGetTokenInformation(ptoken, TokenPrivileges, privs, buf, &buf))
                    {
                        if (_LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &tvalue))
                        {
                            bool hasprivilege = false;

                            for (auto i = 0; i < privs->PrivilegeCount; i++)
                            {
                                if (privs->Privileges[i].Luid.LowPart == tvalue.LowPart && privs->Privileges[i].Luid.HighPart == tvalue.HighPart)
                                {
                                    if (privs->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED)
                                    {
                                        hasprivilege = true;

                                        if (hasprivilege)
                                        {                                           
                                            //  CloseHandle(ptoken);
                                            //  CloseHandle(targetprocess);
                                            //  _memset(&processes, 0, sizeof(processes));
                                        }                                       
                                    }
                                }
                            }                            
                        }
                    }                                                                                         
                }
                LI_FN(CloseHandle)(ptoken);
                LI_FN(CloseHandle)(targetprocess);
            }           
        }       
    }  
    ptrK32EnumProcesses = nullptr;
    ptrOpenProcessToken = nullptr;
    ptrGetTokenInformation = nullptr;
    LI_FN(FreeLibrary)(kernel32);
    LI_FN(FreeLibrary)(advapi32);
    _memset(&processes, 0, sizeof(processes));
}