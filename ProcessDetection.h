#include <map>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <filesystem>


#include "secrets.h"



class ProcessDetection
{
	public:
        
        /// <summary>
        /// Traverses through the process list and compares the file & internal name to ones inside of the NaughtyList.
        /// </summary>
        /// <returns></returns>
        int CheckForProcessesByName();
        /// <summary>
        /// Reads process memory and searches for key strings that are typically inside of the cheat engine process.
        /// </summary>
        /// <param name="processId"></param>
        /// <returns>True if a string has been detected in the processes memory, otherwise false.</returns>
        bool CheatEngineStringsFoundInProcess(DWORD processId);
        

	private:       

        typedef BOOL(WINAPI* DynReadProcessMemory)(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T*);
        StringObfuscation SO;


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