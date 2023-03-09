#include "hwid.h"




void SMBIOSHWID::ShowHwid()
{

    DWORD needBufferSize = 0;
    const DWORD Signature = 'RSMB';
    LPBYTE buffer = NULL;
    
    HMODULE kernel32 = LoadLibraryExA(kernel32dllObf().c_str(), 0, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (kernel32 == NULL)
    {
        std::cout << "Hello, World!" << std::endl;
        return;
    }

    PfGetSystemFirmwareTable ptrGetSystemFirmwareTable = (PfGetSystemFirmwareTable)GetProcAddress(kernel32, GetSystemFirmwareTableObf().c_str());

    if (ptrGetSystemFirmwareTable == nullptr)
    {
        std::cout << "Hello, World!" << std::endl;
        FreeLibrary(kernel32);
    }

    needBufferSize = ptrGetSystemFirmwareTable(Signature, 0, NULL, 0); //  Cold Call
   
    buffer = (LPBYTE)malloc(needBufferSize);

    if (buffer)
    {
        ptrGetSystemFirmwareTable(Signature, 0, buffer, needBufferSize);

        const PRawSMBIOSData pDMIData = (PRawSMBIOSData)buffer;        
        DumpSMBIOSStruct(&(pDMIData->SMBIOSTableData), pDMIData->Length);
    }
    ptrGetSystemFirmwareTable = nullptr;
    FreeLibrary(kernel32);
    buffer = NULL; 
}


const char* SMBIOSHWID::toPointString(void* p){ return (char*)p + ((PSMBIOSHEADER)p)->Length; }


const char* SMBIOSHWID::LocateStringA(const char* str, UINT i)
{

    static const char empty[] = "No Data!";

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


bool SMBIOSHWID::ProcessorDetails(void* p)
{

    PProcessorInfo	pProcessor = (PProcessorInfo)p;
    const char* str = toPointString(p);

    _tprintf(XORWIDE(TEXT("Processor Manufacturer: %s\n")), LocateString(str, pProcessor->Manufacturer));
    uint8_t* id = (uint8_t*)&pProcessor->ID;
    _tprintf(XORWIDE(TEXT("Processor ID: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n")),id[7], id[6], id[5], id[4], id[3], id[2], id[1], id[0]);
    _tprintf(XORWIDE(TEXT("Processor Max Speed: %dMHz\n")), pProcessor->MaxSpeed);
    _tprintf(XORWIDE(TEXT("Processor Current Speed: %dMHz\n")), pProcessor->CurrentSpeed);
    return true;
}


bool SMBIOSHWID::SystemDetails(void* p)
{

    PSystemInfo sys = (PSystemInfo)p;  
    const char* str = toPointString(p);

    if (sys->Header.Length == 0x0)
    {            
        return false;
    }
    if (sys->Header.Length > 0x08)
    {
        _tprintf(XORWIDE(TEXT("UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n")),
            sys->UUID[0], sys->UUID[1], sys->UUID[2], sys->UUID[3],
            sys->UUID[4], sys->UUID[5], sys->UUID[6], sys->UUID[7],
            sys->UUID[8], sys->UUID[9], sys->UUID[10], sys->UUID[11],
            sys->UUID[12], sys->UUID[13], sys->UUID[14], sys->UUID[15]);
    }
    
    if (sys->Header.Length > 0x19)
    {
        _tprintf(XORWIDE(TEXT("Product SKU Number: %s\n")), LocateString(str, sys->SKUNumber));       
    }

    _tprintf(XORWIDE(TEXT("Product Serial Number: %s\n")), LocateString(str, sys->SN));
    _tprintf(XORWIDE(TEXT("Product Manufacturer: %s\n")), LocateString(str, sys->Manufacturer));
    return true;
}


bool SMBIOSHWID::EnclosureDetails(void* p)
{

    PSystemEnclosure enclosure = (PSystemEnclosure)p;
    const char* str = toPointString(p);

    _tprintf(XORWIDE(TEXT("Enclosure Manufacturer: %s\n")), LocateString(str, enclosure->Manufacturer));
    _tprintf(XORWIDE(TEXT("Enclosure Serial Number: %s\n")), LocateString(str, enclosure->SN));
    return true;
}


bool SMBIOSHWID::LogicBoardDetails(void* p)
{

    PBoardInfo pBoard = (PBoardInfo)p;
    const char* str = toPointString(p);
    
    _tprintf(XORWIDE(TEXT("Logic Board Serial Number: %s\n")), LocateString(str, pBoard->SN));
    _tprintf(XORWIDE(TEXT("Logic Board Manufacturer: %s\n")), LocateString(str, pBoard->Manufacturer));
    _tprintf(XORWIDE(TEXT("Logic Board ATN: %s\n")), LocateString(str, pBoard->AssetTag));
    return true;
}


bool SMBIOSHWID::MemoryDeviceDetails(void* p)
{

    PMemoryDevice device = (PMemoryDevice)p;
    const char* str = toPointString(p);

    if (device->Header.Length > 0x15)
    {
        _tprintf(XORWIDE(TEXT("Memory Device Serial Number: %s\n")), LocateString(str, device->SN));
        _tprintf(XORWIDE(TEXT("Memory Device ATN: %s\n")), LocateString(str, device->AssetTag));
        _tprintf(XORWIDE(TEXT("Memory Device Manufacturer: %s\n")), LocateString(str, device->Manufacturer));
    }
    return true;
}


bool SMBIOSHWID::NetworkDetails(void* p)
{

    POnboardDevicesExtendedInformation netdevice = (POnboardDevicesExtendedInformation)p;
    const char* str = toPointString(p);
    
    _tprintf(XORWIDE(TEXT("Network Device Type: %u\n")), netdevice->DeviceType);
    return true;
}


bool SMBIOSHWID::PortableBatteryDetails(void* p)
{

    PPortableBattery battery = (PPortableBattery)p;
    const char* str = toPointString(p);

    if (battery->SN != 0)
    {
        _tprintf(XORWIDE(TEXT("Serial Number: %s\n")), LocateString(str, battery->SN));
    }

    _tprintf(XORWIDE(TEXT("PBattery Manufacturer: %s\n")), LocateString(str, battery->Manufacturer));
    return true;
}
    
    
bool SMBIOSHWID::StartStructureRoutine(PSMBIOSHEADER header)
{
    typedef struct{ BYTE t; bool(*Proc)(void* p);} TPFUNC;

    switch (header->Type)
    {
    case 1:
        SystemDetails((void*)header);
        return true;
    case 2:
        LogicBoardDetails((void*)header);
        return true;
    case 3:
        EnclosureDetails((void*)header);
        return true;
    case 4:
        ProcessorDetails((void*)header);
        return true;
    case 17:
        MemoryDeviceDetails((void*)header);
        return true;
    case 22:
        PortableBatteryDetails((void*)header);
        return true;
    case 41:
        NetworkDetails((void*)header);
        return true;
    default:
        return false;
    }

    return false;
}


void SMBIOSHWID::DumpSMBIOSStruct(void* Addr, UINT Len)
{
  
    LPBYTE p = (LPBYTE)(Addr);
    const LPBYTE lastAddress = p + Len;
    PSMBIOSHEADER header;
    bool isbiosempty = false;
    bool issysempty = false;
    bool isprocessorempty = false;
    bool ismemempty = false;
    bool islogicbempty = false;
    bool isnetdeviceempty = false;
    bool ispbatteryempty = false;

    for (;;)
    {
        header = (PSMBIOSHEADER)p;
        StartStructureRoutine(header);

        switch (header->Type) 
        {
        case 127:
            if (header->Length == 4) 
            {
                return;
            }
           //break;
        case 0:
            if (header->Length == 0 && (isbiosempty == false))
            {
                //std::cout << "\n---\tBIO structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tBIO structure appears to be empty\t---\n")));
                isbiosempty = true;
                //continue;
            }
            //break;
        case 1:
            if (header->Length == 0 && (issysempty == false))
            {
                //std::cout << "\n---\tSYS structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tSYS structure appears to be empty\t---\n")));
                issysempty = true;
                //continue;
            }
            //break;
        case 2:
            if (header->Length == 0 && (islogicbempty == false))
            {
                //std::cout << "\n---\tLOGIC BOARD structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tLOGIC BOARD structure appears to be empty\t---\n")));
                islogicbempty = true;
                //continue;
            }
            //break;
        case 4:
            if (header->Length == 0 && (isprocessorempty == false))
            {
                //std::cout << "\n---\tPROCESSOR structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tPROCESSOR structure appears to be empty\t---\n")));
                isprocessorempty = true;
                //continue;
            }
            //break;
        case 17:
            if (header->Length == 0 && (ismemempty == false))
            {
                //std::cout << "\n---\tMEMORY structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tMEMORY structure appears to be empty\t---\n")));
                ismemempty = true;
                //continue;
            }
            //break;
        case 22:
            if (header->Length == 0 && (ispbatteryempty = false)) 
            {
                //std::cout << "\n---\tPBATTERY structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tBATTERY structure appears to be empty\t---\n")));
                ispbatteryempty = true;
                //continue;
            }
            //break;
        case 41:
            if (header->Length == 0 && (isnetdeviceempty == false))
            {
                //std::cout << "\n---\tNETWORK DEVICE structure appears to be empty\t---\n" << std::endl;
                _tprintf(XORWIDE(TEXT("\n---\tNETWORK DEVICE structure appears to be empty\t---\n")));
                isnetdeviceempty == true;    
                return;
            }
            //break;
        }
        
        LPBYTE nt = p + header->Length;
        while (0 != (*nt | *(nt + 1))) nt++;
        nt += 2;

        if (nt >= lastAddress)
        {
            break;
        }

        p = nt;
    }
}