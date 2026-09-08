#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#include "Log.h"

// ============================================================================
// CopAnims - cop-style weapon animations for the player, as a standalone ASI.
//
// The whole mod is copanims.cpp; this file only opens the log and calls it.
// Everything it touches is found by byte signature at load, so there is no
// game-version check here and nothing to keep in step with a patch level -
// a signature that does not match reports itself in CopAnims.log and that
// half of the feature stays vanilla.
//
// This is the same code that ships inside TACE-Patch as [COPANIMS], lifted
// out with one change: there it borrowed TACE-Patch's own hook on
// CPedMoveBlend::setMoveAnimGroup for the three standing call sites and only
// installed the ducked ones itself. Standing on its own it installs all six.
// ============================================================================

void CopAnims_Init();

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        TaceLog_Init();
        CopAnims_Init();
        TaceLog_Summary();
    }
    return TRUE;
}
