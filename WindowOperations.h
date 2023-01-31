#include <Windows.h>

#include "secrets.h"



class Photography : public StringObfuscation
{
	public:
		

	private:		

		typedef DWORD(WINAPI* DynGetWindowThreadProcessId)(HWND, LPDWORD);	
				
		BOOL CALLBACK EnumWindowsProc(HWND handle, LPARAM lParam);		


};