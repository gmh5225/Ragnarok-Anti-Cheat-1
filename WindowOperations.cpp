#include "WindowOperations.h"






BOOL CALLBACK Photography::EnumWindowsProc(HWND handle, LPARAM lParam)
{

    HMODULE user32 = LoadLibraryA(static_cast<const char*>(User32dll().c_str()));

    if (user32 == NULL)
    {
        return FALSE;
    }
    //  Need obfu.
    DynGetWindowThreadProcessId ptrGetWindowThreadProcessId = (DynGetWindowThreadProcessId)GetProcAddress(user32, "GetWindowThreadProcessId");

    if (ptrGetWindowThreadProcessId == nullptr)
    {
        FreeLibrary(user32);
        return FALSE;
    }

    DWORD pid;

    ptrGetWindowThreadProcessId(handle, &pid);

    DWORD targetpid = (DWORD)lParam;

    if (pid != targetpid)
    {
        FreeLibrary(user32);
        return TRUE;
    }

    bool ismainwindow = GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);

    //  ...

    FreeLibrary(user32);
    return TRUE;
}