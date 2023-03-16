
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include <Psapi.h>

#include "secrets.h"


class ProcessInformer : StringObfuscation
{
	public:

		/// <summary>
		/// Get the thread count of a process.
		/// </summary>
		/// <param name="process">Process ID</param>
		/// <returns>The number of threads, otherwise a value of -1.</returns>
		int GetThreadCount(DWORD process);
		/// <summary>
		/// Get the module count of a process.
		/// </summary>
		/// <param name="process">Process ID</param>
		/// <returns>The number of modules, otherwise a value of -1.</returns>
		int GetModuleCount(DWORD process);
		/// <summary>
		/// Checks if the target process and current process are matching in architecture.
		/// </summary>
		/// <param name="process">Process ID</param>
		/// <returns>True if process are matching the same architecture, otherwise false.</returns>
		inline bool IsTargetArchitectureMatching(DWORD process);
		/// <summary>
		/// Gets the working directory, the directory the process is running from.
		/// </summary>
		/// <param name="process">Process ID</param>
		/// <returns>Workings directory, otherwise an empty string.</returns>
		std::string GetWorkingDirectory(DWORD process);
		std::string GetProcessName(DWORD process);

	private:

		typedef HANDLE(WINAPI* pCreateToolhelp32Snapshot)(DWORD, DWORD);
		StringObfuscation SO;
};
