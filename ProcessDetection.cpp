#include "ProcessDetection.h"



int SuspiciousProcessDetector::CheckForProcessesByName()
{

    DWORD processes[1024], cbNeeded, cProcesses;
    HANDLE targetprocess = INVALID_HANDLE_VALUE;
    std::string processName;
    std::filesystem::path filePath;
    

    if (K32EnumProcesses(processes, sizeof(processes), &cbNeeded))
    {
     
        cProcesses = cbNeeded / sizeof(DWORD);
        
        for (unsigned int i = 0; i < cProcesses; i++)
        {
            if (processes[i] != 0)
            {                              

                targetprocess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);
                
                if (targetprocess == INVALID_HANDLE_VALUE)
                {
                    return -1;
                }                             
                
                char buffer[MAX_PATH] = {};

                K32GetProcessImageFileNameA(targetprocess, buffer, MAX_PATH);

                if (sizeof(buffer) > 0)
                {
                    processName = std::string(buffer);
                    filePath = processName;

                    for (auto iter = debuggers.begin(); iter != debuggers.end(); iter++)
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
                                                                                               
                CloseHandle(targetprocess);
            }
        }
    }

    return 1;
}


bool SuspiciousProcessDetector::CheatEngineStringsFoundInProcess(DWORD processId)
{
    HANDLE process = NULL; 
    
    process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

    if (process == NULL)
    {

        DWORD lastError = GetLastError();

        if (lastError == ERROR_ACCESS_DENIED)
        {
            //  do stuff... Probably going to adjusted privs and attempt to reopen the process with vm read again.
        }

        return false;
    }


    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    LPVOID baseAddress = sysInfo.lpMinimumApplicationAddress;
    LPVOID maxAddress = sysInfo.lpMaximumApplicationAddress;
    size_t found;
    SIZE_T bytesRead;
    const int bufferSize = 4096;
    char buffer[bufferSize];


    while (baseAddress < maxAddress)
    {
        SIZE_T bytesRead;

        if (ReadProcessMemory(process, baseAddress, buffer, bufferSize, &bytesRead))
        {

            std::string memoryContent(buffer, buffer + bytesRead);
           

            for (const auto& currentString : CheatEngineStringFlags)
            {

                found = memoryContent.find(currentString);

                if (found != std::string::npos)
                {
                    std::cout << "FOUND: " << currentString << std::endl;   //  Just for testing
                    CloseHandle(process);
                    sysInfo = {0};
                    return true;
                }
            }
                       
            baseAddress = (LPVOID)((DWORD)baseAddress + bytesRead);
        }
        else
        {
            baseAddress = (LPVOID)((DWORD)baseAddress + bufferSize);
        }
    }

    CloseHandle(process);
    sysInfo = {0};
    return false;
}