#include "ProcessDetection.h"



int ProcessDetection::CheckForProcessesByName()
{

    DWORD processes[1024], cbNeeded, cProcesses;
    std::string processName;
    std::filesystem::path filePath;
    
    if (K32EnumProcesses(processes, sizeof(processes), &cbNeeded))
    {
        cProcesses = cbNeeded / sizeof(DWORD);
        
        for (unsigned int i = 0; i < cProcesses; i++)
        {
            if (processes[i] != 0)
            {                              
                HANDLE targetprocess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);
                
                if (targetprocess == INVALID_HANDLE_VALUE)
                {                   
                    continue;
                }                             
                
                char buffer[MAX_PATH] = {};

                if (K32GetProcessImageFileNameA(targetprocess, buffer, MAX_PATH) != 0)
                {
                    if (sizeof(buffer) > 0)
                    {
                        processName = std::string(buffer);
                        filePath = processName;

                        for (auto iter = NaughtyList.begin(); iter != NaughtyList.end(); iter++)
                        {
                            std::string value = iter->second;
                            std::string internal = filePath.stem().string();

                            if (filePath.filename().string() == value)
                            {
                                //  do stuff
                            }
                            else if ((internal += ".exe") == value)
                            {
                                //  do stuff
                            }
                        }
                    }
                }
                CloseHandle(targetprocess);
            }
            memset(&processes, 0, sizeof(processes));
        }       
    }
    return 1;
}


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

    HMODULE kernel32 = LoadLibraryA(static_cast<const char*>(SO.Kernel32dllObf().c_str()));

    if (kernel32 == INVALID_HANDLE_VALUE || nullptr)
    {
        process = (HANDLE)DecodePointer(process);
        return false;
    }

    DynReadProcessMemory ptrReadProcessMemory = (DynReadProcessMemory)GetProcAddress(kernel32, static_cast<const char*>(SO.ReadProcessMemoryObf().c_str()));

    if (ptrReadProcessMemory == nullptr)
    {
        process = (HANDLE)DecodePointer(process);
        return false;
    }

    CloseHandle(kernel32);

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
                    systeminfo = {};
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
    return false;
}