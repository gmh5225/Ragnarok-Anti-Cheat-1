#include "hwid.h"




void SMBIOSHWID::ShowHwid()
{

    DWORD needBufferSize = 0;
    const DWORD Signature = 'RSMB';
    LPBYTE pBuff = NULL;
    
    needBufferSize = GetSystemFirmwareTable(Signature, 0, NULL, 0); //  Cold Call
    
    pBuff = (LPBYTE)malloc(needBufferSize);

    if (pBuff)
    {
        GetSystemFirmwareTable(Signature, 0, pBuff, needBufferSize);

        const PRawSMBIOSData pDMIData = (PRawSMBIOSData)pBuff;        
        DumpSMBIOSStruct(&(pDMIData->SMBIOSTableData), pDMIData->Length);
    }

    pBuff = NULL; 
}


const char* SMBIOSHWID::toPointString(void* p)
{
    return (char*)p + ((PSMBIOSHEADER)p)->Length;
}


const char* SMBIOSHWID::LocateStringA(const char* str, UINT i)
{

    static const char empty[] = "";

    if (0 == i || 0 == *str) return empty;

    while (--i)
    {
        str += strlen((char*)str) + 1;
    }
    return str;
}


const wchar_t* SMBIOSHWID::LocateStringW(const char* str, UINT i)
{

    static wchar_t buff[2048];
    const char* pStr = LocateStringA(str, i);
    SecureZeroMemory(buff, sizeof(buff));
    const int convSize = MultiByteToWideChar(CP_OEMCP, 0, pStr, (int)strlen(pStr), buff, sizeof(buff) / 2);

    if (convSize > 0)
    {
        return buff;
    }
    return NULL;
}


bool SMBIOSHWID::ProcSysInfo(void* p)
{

    PSystemInfo sys = (PSystemInfo)p;
    
    
    const char* str = toPointString(p);

    if (sys->Header.Length > 0x19)
    {
        //_tprintf(TEXT("SKU Number: %s\n"), LocateString(str, sys->SKUNumber));
        MACHINE::SKU1 = LocateString(str, sys->SKUNumber);
    }

    if (sys->Header.Length > 0x08)
    {
       //_tprintf(TEXT("UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n"),sys->UUID[0], sys->UUID[1], sys->UUID[2], sys->UUID[3],sys->UUID[4], sys->UUID[5], sys->UUID[6], sys->UUID[7],sys->UUID[8], sys->UUID[9], sys->UUID[10], sys->UUID[11],sys->UUID[12], sys->UUID[13], sys->UUID[14], sys->UUID[15]);       

        wchar_t uuid[40];
        swprintf(uuid, 40, TEXT("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"),sys->UUID[0], sys->UUID[1], sys->UUID[2], sys->UUID[3], sys->UUID[4], sys->UUID[5], sys->UUID[6], sys->UUID[7],sys->UUID[8], sys->UUID[9], sys->UUID[10], sys->UUID[11], sys->UUID[12], sys->UUID[13], sys->UUID[14], sys->UUID[15]);
        MACHINE::UUID1 = uuid;
    }

    return true;
}


bool SMBIOSHWID::LogicBoardInfo(void* p)
{

    PBoardInfo pBoard = (PBoardInfo)p;
    const char* str = toPointString(p);
   

    //_tprintf(TEXT("Serial Number: %s\n"), LocateString(str, pBoard->SN));  
    MACHINE::SRN1 = LocateString(str, pBoard->SN);
    return true;
}


bool SMBIOSHWID::DispatchStructType(PSMBIOSHEADER header)
{

    typedef struct
    {
        BYTE t;
        bool(*Proc)(void* p);
    } TPFUNC;

    switch (header->Type)
    {
    case 0:
        ProcSysInfo((void*)header);
        return true;
    case 1:
        LogicBoardInfo((void*)header);
        return true;
    default:
        return false;
    }
}


void SMBIOSHWID::DumpSMBIOSStruct(void* Addr, UINT Len)
{
  
    LPBYTE p = (LPBYTE)(Addr);
    const LPBYTE lastAddress = p + Len;
    PSMBIOSHEADER pHeader;

    for (;;)
    {
        pHeader = (PSMBIOSHEADER)p;
        DispatchStructType(pHeader);

        if ((pHeader->Type == 127) && (pHeader->Length == 4))
        {
            break;
        }

        LPBYTE nt = p + pHeader->Length;
        while (0 != (*nt | *(nt + 1))) nt++;
        nt += 2;

        if (nt >= lastAddress)
        {
            break;
        }

        p = nt;
    }
}