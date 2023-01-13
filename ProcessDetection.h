#include <map>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <filesystem>



class SuspiciousProcessDetector
{
	public:
        
        /// <summary>
        /// Traverses through the process list and compares the file & internal name to ones inside of debuggers map values.
        /// </summary>
        /// <returns></returns>
        int CheckForProcessesByName();
        /// <summary>
        /// Reads process memory and searches for key string that are typically inside of the cheat engine binary.
        /// </summary>
        /// <param name="processId"></param>
        /// <returns>Returns True if Cheat Engine strings have been found in the process, false otherwise.</returns>
        bool CheatEngineStringsFoundInProcess(DWORD processId);
        

	private:       

       
        std::map<std::string, std::string> debuggers = 
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


        std::vector<std::string> CheatEngineStringFlags = 
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
        };
};