#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <SetupAPI.h>
#include <string>


class SystemInformer
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
		/// Runs checks to detect the status of Hyper-V.
		/// </summary>
		/// <returns>True if Hyper-V is enable, False otherwise.</returns>
		bool IsHyperVEnabled();


	private:
		
};