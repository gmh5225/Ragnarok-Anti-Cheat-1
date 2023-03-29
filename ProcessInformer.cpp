#include "ProcessInformer.h"




int ProcessInformer::GetThreadCount(DWORD process)
{

    THREADENTRY32 threadEntry;
    int threadCount = 0;
    DynCreateToolhelp32Snapshot ptrCreateToolhelp32Snapshot = nullptr;

    HMODULE kernel32 = LoadLibraryExA(Kernel32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (kernel32 == INVALID_HANDLE_VALUE) { return -1; }

    ptrCreateToolhelp32Snapshot = (DynCreateToolhelp32Snapshot)GetProcAddress(kernel32, CreateToolhelp32SnapshotObf().c_str());

    if (ptrCreateToolhelp32Snapshot == nullptr)
    {            
        FreeLibrary(kernel32);
        return -1;
    }

    HANDLE snapshotHandle = ptrCreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, process);

    if (snapshotHandle == INVALID_HANDLE_VALUE)
    {
        FreeLibrary(kernel32);
        ptrCreateToolhelp32Snapshot = nullptr;
        return -1;
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
    else
    {
        CloseHandle(snapshotHandle);
        FreeLibrary(kernel32);
        ptrCreateToolhelp32Snapshot = nullptr;
        return -1;
    }

    CloseHandle(snapshotHandle);
    FreeLibrary(kernel32);
    ptrCreateToolhelp32Snapshot = nullptr;
    return threadCount;
}


int ProcessInformer::GetModuleCount(DWORD process)
{
    HMODULE* mods = nullptr;
    DWORD cb = 0;
    int count = -1;

    HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process);

    if (proc == INVALID_HANDLE_VALUE)
    {
        return count;
    }

    if (K32EnumProcessModules(proc, nullptr, 0, &cb))
    {
        mods = new HMODULE[cb / sizeof(HMODULE)];

        if (K32EnumProcessModules(proc, mods, cb, &cb))
        {           
            count = cb / sizeof(HMODULE);           
        }
        else
        {
            return count;
        }
    }
    else
    {
        return count;
    }

    CloseHandle(proc);

    if (mods != nullptr)
    {
        delete[] mods;
    }

    return count;
}


bool ProcessInformer::IsTargetArchitectureMatching(DWORD pid) 
{

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);

    if (process == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BOOL target = FALSE;
    BOOL host = FALSE;

    if (!IsWow64Process(process, &target)) 
    {        
        CloseHandle(process);
        return false;
    }

    IsWow64Process(GetCurrentProcess(), &host);

    CloseHandle(process);

    return (target == host);
}


std::string ProcessInformer::GetWorkingDirectory(DWORD process)
{

    WCHAR buffer[MAX_PATH] = {};
    WCHAR finalPath[sizeof(buffer)];
    DWORD size = sizeof(buffer) ;      
     
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process);

    if (processHandle == INVALID_HANDLE_VALUE)
    {       
        return "";
    }

    DWORD result = QueryFullProcessImageNameW(processHandle, 0, buffer, &size);

    CloseHandle(processHandle);

    if (result == 0)
    {        
        return "";
    }

    HANDLE fileHandle = CreateFileW(buffer, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);

    fileHandle == INVALID_HANDLE_VALUE ? (_RtlSecureZeroMemory(buffer, sizeof(buffer)), "") : "";

    result = GetFinalPathNameByHandleW(fileHandle, finalPath, MAX_PATH, FILE_NAME_NORMALIZED);

    CloseHandle(fileHandle);
  
    result == 0 ? (_RtlSecureZeroMemory(buffer, sizeof(buffer)), "") : "";

    _RtlSecureZeroMemory(buffer, sizeof(buffer));     
    std::wstring widestring(finalPath);
    _RtlSecureZeroMemory(finalPath, sizeof(finalPath));
    std::string fullpath(widestring.begin(), widestring.end());
    fullpath.erase(0, 4);

    size_t lastSlash = fullpath.find_last_of('\\');   

    if (lastSlash == std::string::npos)
    {
        return "";
    }    
    return fullpath.substr(0, lastSlash);
}


std::string ProcessInformer::GetProcessName(DWORD pid)
{

    HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);

    if (processHandle == INVALID_HANDLE_VALUE)
    {
        return "";
    }

    char name[MAX_PATH];

    if (GetProcessImageFileNameA(processHandle, name, MAX_PATH))
    {
        char* nameptr = strrchr(name, '\\');

        if (nameptr)
        {
            nameptr++;
            std::string nameasstring(nameptr);
            CloseHandle(processHandle);
            _RtlSecureZeroMemory(name, sizeof(name));
            return nameasstring;
        }
    }
    CloseHandle(processHandle);
    _RtlSecureZeroMemory(name, sizeof(name));
    return "";
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