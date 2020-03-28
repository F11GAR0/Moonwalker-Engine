#ifndef _GAME_API
#define _GAME_API

#ifndef API
	#if defined(SF_EXPORTS)
		#define API _declspec(dllexport)
	#else
		#define API _declspec(dllimport)
	#endif
#endif

#include "sdk/SharedUtil.h"
#include "game/CGame.h"

extern CGame API *GAME;
extern CPed API *PEDSELF;
#endif