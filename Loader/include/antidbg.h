#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <intrin.h>




const static wchar_t* processmods[] =
{
	L"TitanEngine",
	L"x64_dbg",
	L"Themida",
	L"Qt5Core",
	L"Qt4Core",
	L"ida64",
	L"Scylla",
	L"lua53-64",
	L"tcc64-64"
};


/// <summary>
/// Enums process modules by name, search for matching name.
/// </summary>
/// <param name="pid">Process ID.</param>
/// <returns></returns>
void ScanPidForModules(DWORD pid);
/// <summary>
/// Detects if virtualisation is enabled.
/// </summary>
/// <returns>True if virtualisation is enabled, otherwise false.</returns>
bool VirtualizationEnabled();
