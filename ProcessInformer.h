
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>

#include "secrets.h"


class PI
{
	public:

		int GetThreadCount(DWORD process);
		std::string GetWorkingDirectory(DWORD process);
		std::vector<std::string> GetProcessModules();

	private:

		typedef HANDLE(WINAPI* LPFNCREATETOOLHELP32SNAPSHOT)(DWORD, DWORD);
		StringObfuscation SO;



};
