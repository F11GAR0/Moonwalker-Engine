#pragma once

#pragma warning(disable:4251)

#ifndef API
	#if defined(SF_EXPORTS)
		#define API _declspec(dllexport)
	#else
		#define API _declspec(dllimport)
	#endif
#endif

#if defined(SF_EXPORTS)
typedef CPlugin								*PPLUGIN;
#else
#pragma comment(lib, "SAMPFUNCS.lib")
#pragma comment(lib, "d3dx9.lib")
#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
typedef void								*PPLUGIN;
#endif

class API SFPluginInfo
{
public:
	SFPluginInfo(HMODULE, std::string);
	HMODULE									getPluginHandle(void);
	std::string								getPluginName(void);

private:
	HMODULE									m_hPlugin;
	std::string								m_strPluginName;
};

typedef void(__stdcall *CommandProc) (std::string params);
struct API stCommandInfo
{
	enum CommandType
	{
		NOPE,
		SCRIPT,
		PLUGIN
	};

	stCommandInfo(std::string, CommandType, void*);

	std::string								name;
	CommandType								type;
	union
	{
		struct CScriptThread				*thread;
		class SFPluginInfo					*plugin;
		void								*owner;
	};
};

#include "..\SFAPI\SF_Types.h"
#include "..\SFAPI\SFRender.h"
#include "..\SFAPI\SFRakNet.h"
#include "..\SFAPI\SFSAMP.h"
#include "..\SFAPI\SFCLEO.h"
#include "..\SFAPI\SFGame.h"

class API SAMPFUNCS
{
public:
	PPLUGIN pPlugin;

#if defined( SF_RAKNET )
	SFRakNet								*getRakNet(void);
#endif
#if defined( SF_SAMP )
	SFSAMP									*getSAMP(void);
#endif
#if defined( SF_RENDER )
	SFRender								*getRender(void);
#endif
#if defined( SF_GAME )
	SFGame									*getGame(void);
#endif
#if defined( SF_CLEO )
	SFCLEO									*getCLEO(void);
#endif

	void									Log(const char* pText, ...);
	void									LogFile(const char* pText, ...);
	void									LogConsole(const char* pText, ...);
	bool									initPlugin(void (CALLBACK *PluginThread) (void), HMODULE hModule);
	unsigned int							getAPIVersion(void);
	unsigned int							getSFVersion(void);
	void									registerConsoleCommand(std::string cmd, CommandProc callback);
	void									unregisterConsoleCommand(std::string cmd);
	void									execConsoleCommand(std::string command);
	bool									isConsoleOpened(void);
	bool									isPluginLoaded(std::string strPluginName);
	void									loadPlugin(std::string strPluginName);
	void									unloadPlugin(std::string strPluginName);
	std::vector<stCommandInfo>				getChatCommands(void);
	std::vector<stCommandInfo>				getConsoleCommands(void);
	void									setConsoleCommandDescription(std::string command, std::string description);
	void									Log(const std::string& pText);
};
