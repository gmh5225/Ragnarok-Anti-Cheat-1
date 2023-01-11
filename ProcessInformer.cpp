#include "ProcessInformer.h"




int PI::GetThreadCount(DWORD process)
{

    HANDLE snapshotHandle = {};
    THREADENTRY32 threadEntry = {};
    auto threadCount = 0;


    HMODULE kernel32 = LoadLibraryA(SO.Kernel32dllObf().c_str());

    if (kernel32 == INVALID_HANDLE_VALUE)
    {      
        return 1;
    }

    LPFNCREATETOOLHELP32SNAPSHOT pCreateToolhelp32Snapshot = (LPFNCREATETOOLHELP32SNAPSHOT)GetProcAddress(kernel32, SO.CreateToolhelp32SnapshotObf().c_str());

    if (pCreateToolhelp32Snapshot == nullptr)
    {     
        FreeLibrary(kernel32);
        return 1;
    }

    snapshotHandle = pCreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (snapshotHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    
    threadEntry.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(snapshotHandle, &threadEntry))
    {
        do
        {
            if (threadEntry.th32OwnerProcessID == process)
            {
                threadCount++;
            }
        } while (Thread32Next(snapshotHandle, &threadEntry));
    }

    CloseHandle(snapshotHandle);
    return threadCount;
}


std::string PI::GetWorkingDirectory(DWORD process)
{

    WCHAR buffer[MAX_PATH] = {};
    WCHAR finalPath[sizeof(buffer)];
    DWORD size = sizeof(buffer) ;
    HANDLE fileHandle;
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process);


    if (processHandle == NULL)
    {       
        return "";
    }

    DWORD result = QueryFullProcessImageNameW(processHandle, 0, buffer, &size);

    if (result == 0)
    {        
        CloseHandle(processHandle);
        return "";
    }

    CloseHandle(processHandle);

    fileHandle = CreateFile(buffer, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        return "";
    }

    result = GetFinalPathNameByHandleW(fileHandle, finalPath, MAX_PATH, FILE_NAME_NORMALIZED);

    if (result == 0)
    {        
        CloseHandle(fileHandle);
        return "";
    }

    CloseHandle(fileHandle);

    std::wstring ws(finalPath);
    std::string s(ws.begin(), ws.end());
    s.erase(0, 4);

    size_t lastSlash = s.find_last_of('\\');

    if (lastSlash == std::string::npos)
    {
        return "";
    }

    return s.substr(0, lastSlash);
}


//std::vector<std::string> PI::GetProcessModules()
//{
//    std::vector<std::string> modules;
//    MODULEENTRY32 module;
//    HANDLE capture;
//
//
//    HMODULE kernel32 = LoadLibrary(L"kernel32.dll");
//
//    if (kernel32 == INVALID_HANDLE_VALUE)
//    {
//        return modules;
//    }
//
//    LPFNCREATETOOLHELP32SNAPSHOT pCreateToolhelp32Snapshot = (LPFNCREATETOOLHELP32SNAPSHOT)GetProcAddress(kernel32, "CreateToolhelp32Snapshot");
//
//    if (pCreateToolhelp32Snapshot == nullptr)
//    {
//        if (kernel32 != nullptr)
//        {
//            FreeLibrary(kernel32);
//        } 
//
//        return modules;
//    }
//
//    capture = pCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
//
//    if (capture == INVALID_HANDLE_VALUE)
//    {
//        return modules;
//    }
//
//    module.dwSize = sizeof(module);
//
//    if (Module32First(capture, &module)) 
//    {
//        do 
//        {
//            modules.emplace_back(module.szExePath);      
//        } 
//        while (Module32Next(capture, &module));
//    }
//
//    return modules;
//}