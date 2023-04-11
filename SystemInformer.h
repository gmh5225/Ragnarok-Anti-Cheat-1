//#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <SetupAPI.h>
#include <string>
#include <intrin.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <iphlpapi.h>
#include <bitset>
#include <thread>
#include <vector>
#include <SDKDDKVer.h>

#include "secrets.h"
#include "Resources/Thirdparty/Security/lazy_importer.hpp"

class SystemInformer : StringObfuscation
{
	public:
		
		/// <summary>
		/// Gets the windows install directory, trimmed down to just display the drive letter the OS is installed on.
		/// </summary>
		/// <returns>Example output: C:\</returns>
		std::string GetSystemDriveLetter();
		/// <summary>
		/// Returns the system drive serial number which has been assigned to it via the OS.
		/// </summary>
		/// <returns>System drive serial number. FAIL = empty string.</returns>
		std::string GetSystemDriveSerialNumber();		
		/// <summary>
		/// Checks if Hyperv is enabled.
		/// </summary>
		/// <returns>True if hyperv is enabled, otherwise false.</returns>
		inline bool IsHyperV();
		/// <summary>
		/// Checks if CPU Virtualization is enabled.
		/// </summary>
		/// <returns>True if CPU Virtualization is enabled, otherwise false.</returns>
		inline bool IsCpuVirtualization();
		/// <summary>
		/// Checks if SecureBoot is enabled.
		/// </summary>
		/// <returns>True if SecureBoot is enabled, otherwise false.</returns>
		inline bool IsSecureBoot();
				
	private:
};
