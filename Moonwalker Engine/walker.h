#pragma once
#define WIN32_NO_STATUS
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define NO_SAMPFUNCS
#define LITE_RAKNET

#pragma message( "Compiling precompiled header.\n" )

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

#ifdef NO_SAMPFUNCS
#include "Sequence.h"
#ifdef LITE_RAKNET
typedef unsigned char byte;
#include "LiteRakNet\BitStream.h"
#include "LiteRakNet\RakClient.h"
#include "RakNetController.h"

#else
#include <WinInet.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
typedef unsigned char byte;
#include "raknet\RakClient.h"
#include "raknet\PacketEnumerations.h"
#include "raknet\RakNetworkFactory.h"
#include "raknet\RakClientInterface.h"
#include "raknet\NetworkTypes.h"
#include "raknet\BitStream.h"
#include "raknet\StringCompressor.h"
#include "raknet\SAMP\samp_auth.h"
#endif
#endif


#define LOG_TIMESTAMP

class MOONWALKER {
public:
	MOONWALKER() {
		_log.Delete(this->GetCurrentDllDirectory() + "\\WALKER\\walker.log");
		//m_pRakNet = new RakNetController();
		Log("Session started");
	}
	~MOONWALKER() {
		Log("Session end with no errors.");
	}
	RakNetController *getRakNet() {
		return m_pRakNet;
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
	RakNetController *m_pRakNet;
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




#include "SAMPStructures.h"

#include "SAMPSigcheck.h"
#include "PacketQueue.h"
#include "Exception.h"






BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);