#include "SystemInformer.h"





std::string SystemInformer::GetSystemDriveLetter()
{

	char letter[MAX_PATH];
	std::string sName = "";

	if (GetWindowsDirectoryA(letter, MAX_PATH) == 0)
	{
		return sName;
	}

	sName = reinterpret_cast<char*>(letter);

	return sName.substr(0, 3);
}


std::string SystemInformer::GetSystemDriveSerialNumber()
{

    char systemdrive[4];
    strncpy_s(systemdrive, GetSystemDriveLetter().c_str(), sizeof(systemdrive));
    systemdrive[sizeof(systemdrive) - 1] = 0;

    DWORD serialNum = 0;

    if (sizeof(systemdrive) > 0)
    {
        if (GetVolumeInformationA(systemdrive, nullptr, 0, &serialNum, nullptr, nullptr, nullptr, 260))
        {
            return std::to_string(serialNum);
        }
        else 
        {
            return "";
        }
    }
    return "";
}


wchar_t SystemInformer::GetComputerUUID()
{

    HRESULT setup = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (!SUCCEEDED(setup))
    {
        return '\0';
    }

    IWbemLocator* pLocator = nullptr;

    HRESULT creatobj = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLocator);

    if (!SUCCEEDED(creatobj))
    {
        return '\0';
    }

    IWbemServices* pServices = nullptr;
    pLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pServices);

    
    HRESULT createprox = CoSetProxyBlanket(pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (!SUCCEEDED(createprox))
    {
        return '\0';
    }

    IEnumWbemClassObject* enumerator = nullptr;
    pServices->ExecQuery(_bstr_t(L"WQL"), _bstr_t(L"SELECT * FROM win32_computersystemproduct"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);

    IWbemClassObject* obj = nullptr;
    ULONG uReturn = 0;

    wchar_t result = '\0';

    while (enumerator)
    {
        HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &obj, &uReturn);

        if (uReturn == 0)
        {
            break;
        }              

        _variant_t uuid;

        obj->Get(L"UUID", 0, &uuid, 0, 0);  

        if (SysStringLen(uuid.bstrVal) == 0)
        {
            return '\0';
        }
        else
        {            
            std::wcout << uuid.bstrVal << std::endl;
            result = reinterpret_cast<wchar_t>(uuid.bstrVal);         
        }

        obj->Release();
        break;
    }

    
    enumerator->Release();
    pServices->Release();
    pLocator->Release();
    CoUninitialize();
    
    return result;
}


std::string SystemInformer::GetComputerProcessorId()
{
    return "";
}


bool SystemInformer::IsHyperV()
{

    int regs[4] = { 0 };
    __cpuid(regs, 1);

    return (regs[2] & (1 << 31)) != 0;
}