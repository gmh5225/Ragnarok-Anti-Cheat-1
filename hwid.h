#pragma once
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <inttypes.h>

#include "e115/XOR/xor.h"
#include "e115/Obf/secrets.h"

#ifdef UNICODE
#define getHeaderString  getHeaderStringW
#define LocateString	LocateStringW
#else
#define getHeaderString  getHeaderStringA
#define LocateString	LocateStringA
#endif


class SMBIOSHWID
{
public:
	
	void ShowHwid();

private:
#pragma pack(push)
#pragma pack(1)
	typedef struct _RawSMBIOSData
	{
		BYTE	Used20CallingMethod;
		BYTE	SMBIOSMajorVersion;
		BYTE	SMBIOSMinorVersion;
		BYTE	DmiRevision;
		DWORD	Length;
		PBYTE	SMBIOSTableData;
	} RawSMBIOSData, * PRawSMBIOSData;


	struct RawSMBIOSTable
	{
		BYTE    Type;
		BYTE    Length;
		WORD    Handle;
	};


	typedef struct _SMBIOSHEADER_
	{
		BYTE Type;
		BYTE Length;
		WORD Handle;
	} SMBIOSHEADER, * PSMBIOSHEADER;


	typedef struct _TYPE_1_	//	0
	{
		SMBIOSHEADER	Header;
		UCHAR	Manufacturer;
		UCHAR	ProductName;
		UCHAR	Version;
		UCHAR	SN;
		UCHAR	UUID[16];
		UCHAR	WakeUpType;
		UCHAR	SKUNumber;
		UCHAR	Family;
	} SystemInfo, * PSystemInfo;


	typedef struct _TYPE_2_	//	1
	{
		SMBIOSHEADER	Header;
		UCHAR	Manufacturer;
		UCHAR	Product;
		UCHAR	Version;
		UCHAR	SN;
		UCHAR	AssetTag;
		UCHAR	FeatureFlags;
		UCHAR	LocationInChassis;
		UINT16	ChassisHandle;
		UCHAR	Type;
		UCHAR	NumObjHandle;
		UINT16* pObjHandle;
	} BoardInfo, * PBoardInfo;


	typedef struct _TYPE_3_ 
	{
		SMBIOSHEADER Header;
		UCHAR	Manufacturer;
		UCHAR	Type;
		UCHAR	Version;
		UCHAR	SN;
		UCHAR	AssetTag;
		UCHAR	BootupState;
		UCHAR	PowerSupplyState;
		UCHAR	ThermalState;
		UCHAR	SecurityStatus;
		ULONG32	OEMDefine;
		UCHAR	Height;
		UCHAR	NumPowerCord;
		UCHAR	ElementCount;
		UCHAR	ElementRecordLength;
		UCHAR	pElements;
	} SystemEnclosure, * PSystemEnclosure;


	typedef struct _TYPE_4_ 
	{
		SMBIOSHEADER Header;
		UCHAR	SocketDesignation;
		UCHAR	Type;
		UCHAR	Family;
		UCHAR	Manufacturer;
		ULONG64 ID;
		UCHAR	Version;
		UCHAR	Voltage;
		UINT16	ExtClock;
		UINT16	MaxSpeed;
		UINT16	CurrentSpeed;

	} ProcessorInfo, * PProcessorInfo;


	typedef struct _TYPE_17_ 
	{
		SMBIOSHEADER Header;
		UINT16	PhysicalArrayHandle;
		UINT16	ErrorInformationHandle;
		UINT16	TotalWidth;
		UINT16	DataWidth;
		UINT16	Size;
		UCHAR	FormFactor;
		UCHAR	DeviceSet;
		UCHAR	DeviceLocator;
		UCHAR	BankLocator;
		UCHAR	MemoryType;
		UINT16	TypeDetail;
		UINT16	Speed;
		UCHAR	Manufacturer;
		UCHAR	SN;
		UCHAR	AssetTag;
		UCHAR	PN;
		UCHAR	Attributes;
	} MemoryDevice, * PMemoryDevice;


	typedef struct _TYPE_22_ 
	{
		SMBIOSHEADER Header;
		UCHAR	Location;
		UCHAR	Manufacturer;
		UCHAR	Date;
		UCHAR	SN;
		UCHAR	DeviceName;
		UCHAR   Chemistry;
		UINT16  DesignCapacity;
		UINT16  DesignVoltage;
		UCHAR   SBDSVersionNumber;
		UCHAR   MaximumErrorInBatteryData;
		UINT16  SBDSSerialNumber;
		UINT16	SBDSManufactureDate;
		UCHAR   SBDSDeviceChemistry;
		UCHAR   DesignCapacityMultiplie;
		UINT32  OEM;
	} PortableBattery, * PPortableBattery;

	struct RawSMBIOSSystemInfo : public RawSMBIOSTable
	{
		BYTE    Manufacturer;
		BYTE    ProductName;
		BYTE    Version;
		BYTE    SerialNumber;
		BYTE    UUID[16];
		BYTE    Wakeup_Type;
		BYTE    SKUNumber;
		BYTE    Family;
	};


	typedef struct _TYPE_41_
	{
		SMBIOSHEADER Header;
		UINT8 ReferenceDesignation;
		UINT8 DeviceType;
		UINT16 DeviceTypeInstance;
		UINT8 SegmentGroupNumber;
		UINT8 BusNumber;
		UINT8 DeviceFunctionNumber;
		UINT8 DeviceID;
		UINT32 VendorID;
		UINT8 RevisionID;
		UINT16 Flags;
	} OnboardDevicesExtendedInformation, *POnboardDevicesExtendedInformation;


	typedef UINT(WINAPI* PfGetSystemFirmwareTable)(_In_  DWORD  FirmwareTableProviderSignature,_In_  DWORD  FirmwareTableID,_Out_ PVOID  pFirmwareTableBuffer,_In_  DWORD  BufferSize);


	const char* toPointString(void* p);
	const char* LocateStringA(const char* str, UINT i);
	const wchar_t* LocateStringW(const char* str, UINT i);
	void DumpSMBIOSStruct(void* Addr, UINT Len);
	bool StartStructureRoutine(PSMBIOSHEADER hdr);
	bool SystemDetails(void* p);
	bool LogicBoardDetails(void* p);
	bool ProcessorDetails(void* p);
	bool MemoryDeviceDetails(void* p);
	bool PortableBatteryDetails(void* p);
	bool EnclosureDetails(void* p);
	bool NetworkDetails(void* p);
};