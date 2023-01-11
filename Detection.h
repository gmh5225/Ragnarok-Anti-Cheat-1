#include <map>
#include <string>



class SuspiciousProcessDetector
{
	public:
        
        int CommonDebuggersFound();


	private:

        /// <summary>
        /// Reads process memory common strings found inside of cheat engine -- extra layer of verification:
        /// 
        /// Cheat Engine
        /// CheatEngine
        /// cheatengine
        /// cheat engine
        /// ...
        /// </summary>
        /// <returns>Returns true if process is more than likely a cheat engine.</returns>
        bool IsCheatEngineProcessReal();
        /// <summary>
        /// IDA keeps a history log inside of Computer\HKEY_CURRENT_USER\Software\Hex-Rays\IDA\History
        /// 
        /// I will compare our target process with every entry in this log to determin if IDA has ever had an interaction with our target.
        /// </summary>
        /// <returns>Returns true target process has been found inside of IDA history log.</returns>
        bool IdaOpenSessionWithProcess();

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
            {"Cheat Engine", "CheatEngine.exe"}
        };
};