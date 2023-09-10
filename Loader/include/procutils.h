#pragma once
#include <Windows.h>
#include <winternl.h>
#include <iostream>



//#ifndef _PEB_DEFINED
//#define _PEB_DEFINED
//
//typedef struct _PEB
//{
//    UCHAR InheritedAddressSpace;
//    UCHAR ReadImageFileExecOptions;
//    UCHAR BeingDebugged;
//    UCHAR BitField;
//    ULONG ImageUsesLargePages : 1;
//    ULONG IsProtectedProcess : 1;
//    ULONG IsLegacyProcess : 1;
//    ULONG IsImageDynamicallyRelocated : 1;
//    ULONG SpareBits : 4;
//    PVOID Mutant;
//    PVOID ImageBaseAddress;
//    PPEB_LDR_DATA Ldr;
//    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
//    PVOID SubSystemData;
//    PVOID ProcessHeap;
//    PRTL_CRITICAL_SECTION FastPebLock;
//    PVOID AtlThunkSListPtr;
//    PVOID IFEOKey;
//    ULONG CrossProcessFlags;
//    ULONG ProcessInJob : 1;
//    ULONG ProcessInitializing : 1;
//    ULONG ReservedBits0 : 30;
//    union
//    {
//        PVOID KernelCallbackTable;
//        PVOID UserSharedInfoPtr;
//    };
//    ULONG SystemReserved[1];
//    ULONG SpareUlong;
//    ULONG TlsExpansionCounter;
//    PVOID TlsBitmap;
//    ULONG TlsBitmapBits[2];
//    PVOID ReadOnlySharedMemoryBase;
//    PVOID HotpatchInformation;
//    VOID** ReadOnlyStaticServerData;
//    PVOID AnsiCodePageData;
//    PVOID OemCodePageData;
//    PVOID UnicodeCaseTableData;
//    ULONG NumberOfProcessors;
//    ULONG NtGlobalFlag;
//    LARGE_INTEGER CriticalSectionTimeout;
//    ULONG HeapSegmentReserve;
//    ULONG HeapSegmentCommit;
//    ULONG HeapDeCommitTotalFreeThreshold;
//    ULONG HeapDeCommitFreeBlockThreshold;
//    ULONG NumberOfHeaps;
//    ULONG MaximumNumberOfHeaps;
//    VOID** ProcessHeaps;
//    PVOID GdiSharedHandleTable;
//    PVOID ProcessStarterHelper;
//    ULONG GdiDCAttributeList;
//    PRTL_CRITICAL_SECTION LoaderLock;
//    ULONG OSMajorVersion;
//    ULONG OSMinorVersion;
//    WORD OSBuildNumber;
//    WORD OSCSDVersion;
//    ULONG OSPlatformId;
//    ULONG ImageSubsystem;
//    ULONG ImageSubsystemMajorVersion;
//    ULONG ImageSubsystemMinorVersion;
//    ULONG ImageProcessAffinityMask;
//    ULONG GdiHandleBuffer[34];
//    PVOID PostProcessInitRoutine;
//    PVOID TlsExpansionBitmap;
//    ULONG TlsExpansionBitmapBits[32];
//    ULONG SessionId;
//    ULARGE_INTEGER AppCompatFlags;
//    ULARGE_INTEGER AppCompatFlagsUser;
//    PVOID pShimData;
//    PVOID AppCompatInfo;
//    UNICODE_STRING CSDVersion;  
//    ULONG MinimumStackCommit;
//    LIST_ENTRY FlsListHead;
//    PVOID FlsBitmap;
//    ULONG FlsBitmapBits[4];
//    ULONG FlsHighIndex;
//    PVOID WerRegistrationData;
//    PVOID WerShipAssertPtr;
//
//} PEB, * PPEB;
//
//#endif

using TNtQueryInformationProcess = NTSTATUS(NTAPI*)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);


FARPROC GetFunctionAddressW(HMODULE moduleHandle, const wchar_t* method);
void SecureErase(void* data, size_t size);
PEB* PebBaseAddress();