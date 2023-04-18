#pragma once


#include <Windows.h>
#include <vector>
#include <SoftPub.h>
#include <wincrypt.h>

#include "..\Ragnarok\Core\Foundation\Security\Text\stringobf.h"
#include "..\Ragnarok\Core\Foundation\Security\Functions\ImportObfuscation.h"
#include "..\Ragnarok\Core\Foundation\Security\Text\xor.h"


class ProcessDetection : StringObfuscation
{
	public:
               
        /// <summary>
        /// Traverses through the process list and checks if process has SeDebugPriviledgeToken, excluding ourself.
        /// </summary>
        void DetectSeDebugPrivilegeToken();       

	private:     

        enum RETURN_CODES : int
        {
            /// <summary>
            /// If routine is executed like normal.
            /// </summary>
            RanEnd = -1,
            FailedGetProc = 1,
            FailedLoadLib,
            FailedProcHandle
        };


        /// <summary>
        /// Reads process memory and searches for key strings that are typically inside of the cheat engine process.
        /// </summary>
        /// <param name="processId"></param>
        /// <returns></returns>
        int CheatEngineStringsFoundInProcess(DWORD processId);       


        typedef BOOL(WINAPI* DynReadProcessMemory)(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T*);
        typedef BOOL(WINAPI* Dyn32EnumProcesses)(DWORD*, DWORD, DWORD*);
        typedef BOOL(WINAPI* LPFN_GETTOKENINFORMATION)(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);       
        typedef BOOL(WINAPI* DynGetTokenInformation)(HANDLE TokenHandle,TOKEN_INFORMATION_CLASS TokenInformationClass,LPVOID TokenInformation,DWORD TokenInformationLength,PDWORD ReturnLength);
        typedef BOOL(WINAPI* DynOpenProcessToken)(HANDLE ProcessHandle,DWORD DesiredAccess,PHANDLE TokenHandle);
     

        std::vector<std::string> CheatEngineString = 
        {
            "Cheat Engine",
            "cheat engine",
            "Cheat engine",
            "cheat Engine",
            "CHEAT ENGINE",
            "CHeat Engine",
            "CheAt Engine",
            "Cheat EnginE",
            "CheatEngine",
            "cheatengine",
            "Cheatengine",
            "cheatEngine",
            "CHEATENGINE",
            "CHeatEngine",
            "CheatEngine",
            "CET_TRAINER.CETRAINER",
            ".CETRAINER",
            "TCEThread"
        };
};