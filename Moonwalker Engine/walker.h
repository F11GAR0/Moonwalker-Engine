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


#define LOG_TIMESTAMP

class MOONWALKER {
public:
	MOONWALKER() {
		_log.Delete(this->GetCurrentDllDirectory() + "\\WALKER\\walker.log");
		Log("Session started");
	}
	~MOONWALKER() {
		Log("Session end with no errors.");
	}
	void Log(std::string message) {
		SYSTEMTIME time;
		GetLocalTime(&time);
		_log.Open(this->GetCurrentDllDirectory() + "\\WALKER\\walker.log", Mode::M_OPEN_WRITE_APPEND);
		_log.Write(
#ifdef LOG_TIMESTAMP
			"[" + Convert::ToString(time.wDay)
			+ ":" + Convert::ToString(time.wMonth)
			+ ":" + Convert::ToString(time.wYear)
			+ " || " + Convert::ToString(time.wHour)
			+ ":" + Convert::ToString(time.wMinute)
			+ ":" + Convert::ToString(time.wSecond)
			+ ":" + Convert::ToString(time.wMilliseconds)
			+ "]: " +
#endif
			message + '\n');
		_log.Close();
	}
private:
	FileWriter _log;
	std::string GetCurrentDllDirectory()
	{
		char result[MAX_PATH];
		std::string ret = std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
		bool stop = false;
		while (!stop) {
			if (ret[ret.size() - 1] != '\\')
				ret.erase(ret.end() - 1);
			else stop = true;
		}
		return ret;
	}
};

#ifdef NO_SAMPFUNCS
MOONWALKER *NJIN = new MOONWALKER();
#else
SAMPFUNCS *NJIN;
#endif

#include "SAMPSigcheck.h"
#include "PacketQueue.h"
#include "Exception.h"
#include "Sequence.h"




BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);