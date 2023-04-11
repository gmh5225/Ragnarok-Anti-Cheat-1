#include "SystemInformer.h"



std::string SystemInformer::GetSystemDriveLetter()
{

	char letter[MAX_PATH];
	std::string sName = "";

    auto result = LI_FN(GetWindowsDirectoryA)(letter, MAX_PATH);

	if (result == 0)
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
        if (LI_FN(GetVolumeInformationA)(systemdrive, nullptr, 0, &serialNum, nullptr, nullptr, nullptr, 260))
        {
            return std::to_string(serialNum);
        }
        else 
        {
            return "";
        }
    }
    return "";
}


inline bool SystemInformer::IsHyperV()
{

    int regs[4] = { 0 };
    __cpuid(regs, 1);

    return (regs[2] & (1 << 31)) != 0;
}


inline bool SystemInformer::IsCpuVirtualization()
{

    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 1);

    std::bitset<32> ECX(CPUInfo[2]);

    if (ECX[5]) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}


inline bool SystemInformer::IsSecureBoot()
{

    char brandString[49];
    __cpuid(reinterpret_cast<int*>(brandString), 0x80000002);
    __cpuid(reinterpret_cast<int*>(brandString + 16), 0x80000003);
    __cpuid(reinterpret_cast<int*>(brandString + 32), 0x80000004);

    return (strstr(brandString, SecureBoot().c_str()) != nullptr) ? true : false;
}
