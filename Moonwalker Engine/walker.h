#pragma once
#define WIN32_NO_STATUS
#include <Windows.h>
#pragma warning (disable:4700)
#pragma warning (disable:4244)
#pragma warning (disable:4018)
#pragma warning (disable:4005)
#include "SAMPFUNCS\game_api\game_api.h"
#include "SAMPFUNCS\SFAPI\SAMPFUNCS_API.h"

SAMPFUNCS *SF;

#include <iostream>
#include <string>
#include <tuple>
#include "PacketQueue.h"
#include "Exception.h"
#include "Sequence.h"


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);