
#include <windows.h>
#include <TlHelp32.h>
#include <string.h>
#include <iostream>


class PI
{
	public:

		int GetThreadCount(DWORD process);
		std::string GetWorkingDirectory(DWORD process);

	private:

		typedef HANDLE(WINAPI* LPFNCREATETOOLHELP32SNAPSHOT)(DWORD, DWORD);



};
