#include "pch.h"

#include "Widescreen.h"
#include "MemoryMgr.h"
#include "ini.h"


void Widescreen::Initialise() 
{
    ScopedUnprotect::Section Protect(GetModuleHandle(nullptr), ".rdata");

    mINI::INIFile file("Widescreen.ini");
    mINI::INIStructure ini;

    file.read(ini);

    int ResWidth = 0;
    int ResHeight = 0;
    DWORD BufferSize = 8192;
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\THQ\\Juiced\\", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
    {
        RegGetValue(hKey, NULL, L"RezWidth", RRF_RT_REG_DWORD, NULL, (PVOID)&ResWidth, &BufferSize);
        RegGetValue(hKey, NULL, L"RezHeight", RRF_RT_REG_DWORD, NULL, (PVOID)&ResHeight, &BufferSize);
        
        RegCloseKey(hKey);
    }

    if (ResHeight == 0 || ResWidth == 0)
        return;

    float AspectRatio = static_cast<float>(ResWidth) / ResHeight;
    int sFov = atoi(ini["WIDESCREEN"]["CustomFov"].c_str());
    float fov = strtof(ini["WIDESCREEN"]["FOV"].c_str(), NULL);

    if (sFov == 0)
        fov = 0;
    
    if (AspectRatio == 16.0f / 9.0f) // 16:9
        SetAspectRatio(1, fov);
    else if (AspectRatio == 15.0f / 9.0f) // 15:9
        SetAspectRatio(1.066666, fov);
    else if (AspectRatio == 21.0f / 9.0f) // 21:9
        SetAspectRatio(0.8, fov);
}

void Widescreen::SetAspectRatio(float aspectRatio, float fov) 
{
    Memory::Patch<float>(0x72477C, aspectRatio);

    if (fov >= 1.0f || fov <= 2.0f)
        Memory::Patch<float>(0x6FEAF0, fov);
}