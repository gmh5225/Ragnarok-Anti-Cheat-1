#include "SystemInformer.h"





std::string SystemInformer::GetSystemDriveLetter()
{

	char letter[MAX_PATH];
	std::string sName = "";

	if (GetWindowsDirectoryA(letter, MAX_PATH) == 0)
	{
		return sName;
	}

	sName = reinterpret_cast<char*>(letter);

	return sName.substr(0, 3);
}


std::string SystemInformer::GetSystemDriveSerialNumber()
{

    char systemdrive[4];
    strncpy_s(systemdrive, GetSystemDriveLetter().c_str(), sizeof(systemdrive));
    systemdrive[sizeof(systemdrive) - 1] = 0;

    DWORD serialNum = 0;

    if (sizeof(systemdrive) > 0)
    {
        if (GetVolumeInformationA(systemdrive, nullptr, 0, &serialNum, nullptr, nullptr, nullptr, 260))
        {
            return std::to_string(serialNum);
        }
        else 
        {
            //
        }
    }

    return "";
}