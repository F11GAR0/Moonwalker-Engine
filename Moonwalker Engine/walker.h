#pragma once
#include <Windows.h>
#include "SAMPFUNCS\SFAPI\SAMPFUNCS_API.h"

SAMPFUNCS *SF;

#include <iostream>
#include <string>
#include <tuple>
#include "PacketQueue.h"
#include "Exception.h"
#include "Sequence.h"


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);