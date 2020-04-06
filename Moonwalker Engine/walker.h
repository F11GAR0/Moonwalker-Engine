#ifndef WALKER_MAIN
#define WALKER_MAIN

#define WIN32_NO_STATUS
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define NO_SAMPFUNCS
#define LITE_RAKNET
#define LOG_TIMESTAMP

#define MOONWALKER_VERSION "1.0"

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
#include "PacketQueue.h"
#include "WalkerBase.h"


#ifdef NO_SAMPFUNCS
extern class MOONWALKER *NJIN;
#else
extern class SAMPFUNCS *NJIN;
#endif


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
#else

#endif