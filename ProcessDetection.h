#pragma once

#include <map>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <filesystem>
#include <Sddl.h>
#include <vector>

#include "secrets.h"
#include "ImportObfuscation.h"


class ProcessDetection : public StringObfuscation
{
	public:
        
        /// <summary>
        /// Traverses through the process list and compares the file & internal name to ones inside of the NaughtyList.
        /// </summary>
        /// <returns>-1 on success, otherwise 1 = GetProcAddress Fails, 2 = LoadLibrary Fails, 3 = Getting Process Handle Fails.</returns>
        // int CheckForProcessesByName();
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
        /// <returns>True if a string has been detected in the processes memory, otherwise false.</returns>
        bool CheatEngineStringsFoundInProcess(DWORD processId);


        typedef BOOL(WINAPI* DynReadProcessMemory)(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T*);
        typedef BOOL(WINAPI* Dyn32EnumProcesses)(DWORD*, DWORD, DWORD*);
        typedef BOOL(WINAPI* LPFN_GETTOKENINFORMATION)(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);       
        typedef BOOL(WINAPI* DynGetTokenInformation)(HANDLE TokenHandle,TOKEN_INFORMATION_CLASS TokenInformationClass,LPVOID TokenInformation,DWORD TokenInformationLength,PDWORD ReturnLength);
        typedef BOOL(WINAPI* DynOpenProcessToken)(HANDLE ProcessHandle,DWORD DesiredAccess,PHANDLE TokenHandle);


        std::map<std::string, std::string> NaughtyList = 
        {
            {"IDA Pro", "ida.exe"},
            {"OllyDbg", "ollydbg.exe"},
            {"x64dbg", "x64dbg.exe"},
            {"Radare2", "radare2"},
            {"Binary Ninja", "binaryninja.exe"},
            {"Ghidra", "ghidraRun.bat"},
            {"Hopper", "hopper.exe"},
            {"IDA Free", "ida.exe"},
            {"Hex-Rays Decompiler", "hexrays.exe"},
            {"ImmDbg", "windbg.exe"},
            {"Cheat Engine", "CheatEngine.exe"},
            {"Process Hacker", "ProcessHacker.exe"},
            {"CFF Explorer", "CFF Explorer.exe"}
        };


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