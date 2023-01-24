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
		/// Captures current machines UUID.
		/// </summary>
		/// <returns>Computer UUID, Otherwise '\0'</returns>
		wchar_t GetComputerUUID();
		/// <summary>
		/// Captures processor ID.
		/// </summary>
		/// <returns>Processor ID, Otherwise Empty string.</returns>
		std::string GetComputerProcessorId();		
		bool IsHyperV();
		

	private:
		
		
};
