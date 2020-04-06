#pragma once
/*
TODO:
- Setup SAMP Hooks (pools, raknet, etc)
- Bound RakNetController callbacks and this shit
*/
#include "walker.h"


#define SAMP_DLL "samp.dll"

#define SAMP_R2SIGNATURE "F8036A004050518D4C24"
#define SAMP_R3SIGNATURE "E86D9A0A0083C41C85C0"



enum SAMPVER {
	UNKNOWN = -13,
	V037R2 = 1, 
	V037R3
};


class POOLS {

};

class SAMP {
public:
	uint32_t GetSampDllAddress();
	SAMPVER CheckVersion();
	SAMP();
	RakNetController *getRakNet();
	void LoadSAMPPoolSystem(SAMPVER ver);
private:
	RakNetController		   *m_pRakNet;
	
	STRUCTSV037R2::stSAMP			*m_stSAMPV037R2;
	STRUCTSV037R2::pool				*m_stActorPool;
	STRUCTSV037R2::pool				*m_stVehiclePool;

	STRUCTSV037R3::stSAMP			*m_stSAMPV037R3;
};