#include "pch.h"
#include "mhDetect.h"
unsigned int dwGameDll = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (WarcraftVersion() == 52240) {
            DisableThreadLibraryCalls(hModule);
            dwGameDll = (unsigned int)GetModuleHandle(L"Game.dll");
            jass::init();
            mhDetect::init();
        }
        else {
            MessageBoxW(NULL, L"Support WarCraft III 1.27 only", L"Warning", MB_ICONSTOP | MB_APPLMODAL | MB_TOPMOST);
            return false;
        }
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

