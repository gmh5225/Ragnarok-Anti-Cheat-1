#include "ProcessDetection.h"




//int ProcessDetection::CheckForProcessesByName()
//{
//
//    DWORD processes[1024], cbNeeded, cProcesses;
//    std::string processName;
//    std::filesystem::path filePath;
//    HMODULE kernel32 = LoadLibraryExA(Kernel32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);
//    Dyn32EnumProcesses ptrK32EnumProcesses = nullptr;
//
//    if (kernel32 == INVALID_HANDLE_VALUE)
//    {
//        return FailedLoadLib;
//    }
//
//    ptrK32EnumProcesses = (Dyn32EnumProcesses)GetProcAddress(kernel32, K32EnumProcessesObf().c_str());
//
//    if (ptrK32EnumProcesses == nullptr)
//    {
//        FreeLibrary(kernel32);
//        return FailedGetProc;
//    }
//
//    if (ptrK32EnumProcesses(processes, sizeof(processes), &cbNeeded))
//    {
//        cProcesses = cbNeeded / sizeof(DWORD);
//        
//        for (unsigned int i = 0; i < cProcesses; i++)
//        {
//            if (processes[i] != 0)
//            {                              
//                HANDLE targetprocess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);
//                
//                if (targetprocess == INVALID_HANDLE_VALUE)
//                {                   
//                    continue;
//                }                             
//                
//                char buffer[MAX_PATH] = {};
//
//                if (K32GetProcessImageFileNameA(targetprocess, buffer, MAX_PATH) != 0)
//                {
//                    if (sizeof(buffer) > 0)
//                    {
//                        processName = std::string(buffer);
//                        filePath = processName;
//
//                        for (auto iter = NaughtyList.begin(); iter != NaughtyList.end(); iter++)
//                        {
//                            std::string value = iter->second;
//                            std::string internal = filePath.stem().string();
//
//                            if (filePath.filename().string() == value)
//                            {
//                                TerminateProcess(targetprocess, 0);
//                                CloseHandle(targetprocess);
//                            }
//                            else if ((internal += ".exe") == value)
//                            {
//                                TerminateProcess(targetprocess, 0);
//                                CloseHandle(targetprocess);
//                            }
//                        }
//                    }
//                }
//                CloseHandle(targetprocess);
//            }            
//        }  
//        memset(&processes, 0, sizeof(processes));
//    }
//    ptrK32EnumProcesses = nullptr;
//    CloseHandle(kernel32);
//    FreeLibrary(kernel32);
//    return RanEnd;
//}


bool ProcessDetection::CheatEngineStringsFoundInProcess(DWORD processId)
{
    
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

    if (process == INVALID_HANDLE_VALUE)
    {               
        return false;
    }

    process = (HANDLE)EncodePointer(process);

    SYSTEM_INFO systeminfo = {};
    GetSystemInfo(&systeminfo);
    LPVOID base = systeminfo.lpMinimumApplicationAddress;
    LPVOID max = systeminfo.lpMaximumApplicationAddress;
    size_t found;
    const int buffsize = 8096;
    char buffer[buffsize];

    HMODULE kernel32 = LoadLibraryA(Kernel32dllObf().c_str());

    if (kernel32 == INVALID_HANDLE_VALUE || nullptr)
    {
        process = (HANDLE)DecodePointer(process);
        CloseHandle(process);
        return false;
    }

    DynReadProcessMemory ptrReadProcessMemory = (DynReadProcessMemory)GetProcAddress(kernel32, ReadProcessMemoryObf().c_str());

    if (ptrReadProcessMemory == nullptr)
    {
        FreeLibrary(kernel32);
        process = (HANDLE)DecodePointer(process);
        CloseHandle(process);
        return false;
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
                    CloseHandle(kernel32);
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

    HMODULE kernel32 = LoadLibraryExA(Kernel32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (kernel32 == INVALID_HANDLE_VALUE) { return; }

    HMODULE advapi32 = LoadLibraryExA(Advapi32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (advapi32 == NULL)
    {
        FreeLibrary(kernel32);
        ptrK32EnumProcesses = nullptr;
        return;
    }

    FARPROC ptrGetProcAddress = FindGetProcAddress();

    ptrK32EnumProcesses = (Dyn32EnumProcesses)((FARPROC(*)(HMODULE, LPCSTR))ptrGetProcAddress)(kernel32, K32EnumProcessesObf().c_str());
    //  ptrK32EnumProcesses = (Dyn32EnumProcesses)GetProcAddress(kernel32, K32EnumProcessesObf().c_str());

    if (ptrK32EnumProcesses == nullptr)
    {
        FreeLibrary(kernel32);
        FreeLibrary(advapi32);
        return;
    }

    if (ptrK32EnumProcesses(processes, sizeof(processes), &cbNeeded))
    {      
        ptrOpenProcessToken = (DynOpenProcessToken)((FARPROC(*)(HMODULE, LPCSTR))ptrGetProcAddress)(advapi32, OpenProcessTokenObf().c_str());
        //  ptrOpenProcessToken = (DynOpenProcessToken)GetProcAddress(advapi32, OpenProcessTokenObf().c_str());
        if (ptrOpenProcessToken == nullptr) 
        {            
            FreeLibrary(kernel32);
            FreeLibrary(advapi32);    
            ptrK32EnumProcesses = nullptr;            
            return;
        }

        ptrGetTokenInformation = (DynGetTokenInformation)((FARPROC(*)(HMODULE, LPCSTR))ptrGetProcAddress)(advapi32, GetTokenInformationObf().c_str());
        //  ptrGetTokenInformation = (DynGetTokenInformation)GetProcAddress(advapi32, GetTokenInformationObf().c_str());
        if (ptrGetTokenInformation == nullptr) 
        { 
            FreeLibrary(kernel32);
            FreeLibrary(advapi32);
            ptrK32EnumProcesses = nullptr;
            ptrOpenProcessToken = nullptr;
            return;
        }

        cProcesses = cbNeeded / sizeof(DWORD);

        for (unsigned int i = 0; i < cProcesses; i++)
        {
            if (processes[i] != 0)
            {
                HANDLE targetprocess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);

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
                        if (LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &tvalue))
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
                                        }                                       
                                    }
                                }
                            }                            
                        }
                    }                                                                                         
                }
                CloseHandle(ptoken);
                CloseHandle(targetprocess);
            }           
        }       
    }  
    ptrK32EnumProcesses = nullptr;
    ptrOpenProcessToken = nullptr;
    ptrGetTokenInformation = nullptr;
    FreeLibrary(kernel32);
    FreeLibrary(advapi32);
    memset(&processes, 0, sizeof(processes));
}