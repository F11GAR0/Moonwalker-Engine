#pragma once
#define WIN32_NO_STATUS

#define NO_SAMPFUNCS

#include <Windows.h>
#pragma warning (disable:4700)
#pragma warning (disable:4244)
#pragma warning (disable:4018)
#pragma warning (disable:4005)

#ifndef NO_SAMPFUNCS
#include "SAMPFUNCS\game_api\game_api.h"
#include "SAMPFUNCS\SFAPI\SAMPFUNCS_API.h"
#endif // NO_SAMPFUNCS



#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <tuple>

#include "File.h"
#include "Converter.h"

class MOONWALKER {
public:
	MOONWALKER() {
		_log.Open(this->GetCurrentDllDirectory() + "\\WALKER\\walker.log", Mode::M_TRUNC);
		Log("Session started");
	}
	void Log(std::string message) {
		SYSTEMTIME time;
		GetLocalTime(&time);
		_log.Write("[" + Convert::ToString(time.wDay)
			+ ":" + Convert::ToString(time.wMonth)
			+ ":" + Convert::ToString(time.wYear)
			+ " || " + Convert::ToString(time.wHour)
			+ ":" + Convert::ToString(time.wMinute)
			+ ":" + Convert::ToString(time.wSecond)
			+ ":" + Convert::ToString(time.wMilliseconds)
			+ "]: "
			+ message);
	}
private:
	FileWriter _log;
	std::string GetCurrentDllDirectory()
	{
		char result[MAX_PATH];
		return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
	}
};

#ifdef NO_SAMPFUNCS
MOONWALKER *NJIN;
#else
SAMPFUNCS *NJIN;
#endif

#include "PacketQueue.h"
#include "Exception.h"
#include "Sequence.h"




BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);