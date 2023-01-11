#include "Detection.h"



int SuspiciousProcessDetector::EasyCheckForSusProcesses()
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

                        if (filePath.filename().string() == value)
                        {
                            //  TODO
                        }
                    }
                }                
                                                                                               
                CloseHandle(targetprocess);
            }
        }
    }

    return 0;
}