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


//typedef struct _RawSMBIOSData
//{
//	BYTE	Used20CallingMethod;
//	BYTE	SMBIOSMajorVersion;
//	BYTE	SMBIOSMinorVersion;
//	BYTE	DmiRevision;
//	DWORD	Length;
//	PBYTE	SMBIOSTableData;
//} RawSMBIOSData, * PRawSMBIOSData;
//
//struct RawSMBIOSTable
//{
//	BYTE    Type;
//	BYTE    Length;
//	WORD    Handle;
//};
//
//typedef struct _SMBIOSHEADER_
//{
//	BYTE Type;
//	BYTE Length;
//	WORD Handle;
//} SMBIOSHEADER, * PSMBIOSHEADER;
//
//typedef struct _TYPE_1_ {
//	SMBIOSHEADER	Header;
//	UCHAR	Manufacturer;
//	UCHAR	ProductName;
//	UCHAR	Version;
//	UCHAR	SN;
//	UCHAR	UUID[16];
//	UCHAR	WakeUpType;
//	UCHAR	SKUNumber;
//	UCHAR	Family;
//} SystemInfo, * PSystemInfo;
//
//typedef struct _TYPE_2_ {
//	SMBIOSHEADER	Header;
//	UCHAR	Manufacturer;
//	UCHAR	Product;
//	UCHAR	Version;
//	UCHAR	SN;
//	UCHAR	AssetTag;
//	UCHAR	FeatureFlags;
//	UCHAR	LocationInChassis;
//	UINT16	ChassisHandle;
//	UCHAR	Type;
//	UCHAR	NumObjHandle;
//	UINT16* pObjHandle;
//} BoardInfo, * PBoardInfo;
//
//
//struct RawSMBIOSSystemInfo : public RawSMBIOSTable
//{
//	BYTE    Manufacturer;
//	BYTE    ProductName;
//	BYTE    Version;
//	BYTE    SerialNumber;
//	// Ver 2.1 beyond here
//	BYTE    UUID[16];
//	BYTE    Wakeup_Type;
//	// Ver 2.4 beyond here
//	BYTE    SKUNumber;
//	BYTE    Family;
//};
//
//void DumpSMBIOSStruct(void* Addr, UINT Len);

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
		/// Checks if Hyperv is enabled.
		/// </summary>
		/// <returns>True if hyperv is enabled, otherwise false.</returns>
		inline bool IsHyperV();
		/// <summary>
		/// Checks if CPU Virtualization is enabled.
		/// </summary>
		/// <returns>True if CPU Virtualization is enabled, otherwise false.</returns>
		inline bool IsCpuVirtualization();
		/*void GetComputerProcessorIdSMBIOS();*/

		//bool ProcSysInfo(void* p);
		/*bool DispatchStructType(PSMBIOSHEADER hdr);
		const char* toPointString(void* p);
		const char* LocateStringA(const char* str, UINT i);
		const wchar_t* LocateStringW(const char* str, UINT i);*/

		
	private:
		
		
};
