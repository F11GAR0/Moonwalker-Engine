#include "walker.h"


#define MOONWALKER_VERSION "1.0"

Sequence<std::tuple<std::string, HMODULE>> *plugins = new Sequence<std::tuple<std::string, HMODULE>>();

void _stdcall LoadPlugin(std::string name) {
	char *buff = (char*)malloc(512);
	GetCurrentDirectory(512, buff);
	char *path = (char*)malloc(512);
	sprintf(path, "%s/%s", buff, name.c_str());
	free(buff);
	std::tuple<std::string, HMODULE> plugin = std::make_tuple(name, LoadLibrary(path));
	free(buff);
	if (!std::get<1>(plugin)) {
		Exception *ex = new Exception("LoadPlugin", 2, (std::string("Failed to load plugin from ") + name).c_str());
		ex->Send();
		delete ex;
	}
	else {
		plugins->Push(plugin);
	}
}

void _stdcall FreePlugin(std::string name) {
	for (int i = 0; i < plugins->Count(); i++) {
		if (std::get<0>((*plugins)[i]) == name) {
			FreeLibrary(std::get<1>((*plugins)[i]));
			plugins->Delete(i);
		}
	}
}

void LoadPlugins() {
	
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SF->registerConsoleCommand("walkerload", LoadPlugin);
		SF->registerConsoleCommand("walkerfree", FreePlugin);
		SF->Log("[MOONWALKER]: Moonwalker loaded. Version: " + std::string(MOONWALKER_VERSION));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}