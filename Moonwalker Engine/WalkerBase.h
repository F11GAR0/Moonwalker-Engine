#pragma once
#ifndef WALKER_MAIN
#error "блять залупа мейн не заинклудили!"
#endif
#include "walker.h"
#include "SAMPStructures.h"
#include "SAMPSigcheck.h"

class MOONWALKER {
public:
	MOONWALKER();
	~MOONWALKER();
	SAMP* getSAMP();
	void Log(std::string message);
private:
	FileWriter m_fLog;
	SAMP* m_SAMP;
	std::string GetCurrentDllDirectory();
};


