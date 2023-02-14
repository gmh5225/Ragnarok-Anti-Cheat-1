#pragma once
#include <Windows.h>
#include <iostream>
#include <tchar.h>






namespace MACHINE 
{
	const wchar_t* SRN1;
	const wchar_t* UUID1;
	const wchar_t* SKU1;
}


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
	/*void DumpSMBIOSStruct(void* Addr, UINT Len);*/
private:

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


	typedef struct _TYPE_1_
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


	typedef struct _TYPE_2_
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

	const char* toPointString(void* p);
	const char* LocateStringA(const char* str, UINT i);
	const wchar_t* LocateStringW(const char* str, UINT i);
	void DumpSMBIOSStruct(void* Addr, UINT Len);
	bool DispatchStructType(PSMBIOSHEADER hdr);
	bool ProcSysInfo(void* p);
	bool LogicBoardInfo(void* p);
};


