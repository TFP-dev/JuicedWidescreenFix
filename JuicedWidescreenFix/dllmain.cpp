// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Widescreen.h"
#include "MemoryMgr.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        Widescreen::Initialise();
        
        return TRUE;
    }
    
    return FALSE;
}

