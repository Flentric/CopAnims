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
// It hooks all six CPedMoveBlend::setMoveAnimGroup call sites itself - the
// three standing ones and the three ducked ones.
// ============================================================================

void CopAnims_Init();

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        Log_Init();
        CopAnims_Init();
        Log_Summary();
    }
    return TRUE;
}
