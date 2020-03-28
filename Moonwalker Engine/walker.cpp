#include "walker.h"


#define MOONWALKER_VERSION "1.0"

Sequence<std::tuple<std::string, HMODULE>> *plugins = new Sequence<std::tuple<std::string, HMODULE>>();

void _stdcall LoadPlugin(std::string name) {
	SF->Log("[MOONWALKER]: Trying to load plugin: " + name);
	char *buff = (char*)malloc(512);
	GetCurrentDirectory(512, buff);
	char *path = (char*)malloc(512);
	sprintf(path, "%s\\WALKER\\%s", buff, name.c_str());
	free(buff);
	std::tuple<std::string, HMODULE> plugin = std::make_tuple(name, LoadLibrary(path));
	if (std::get<1>(plugin) != NULL) {
		free(path);
		plugins->Push(plugin);
		SF->Log("[MOONWALKER]: plugin \"" + name + "\" succesfuly loaded");
	}
	else {
		Exception *ex = new Exception("LoadPlugin", 2, (std::string("Failed to load plugin from ") + path).c_str());
		ex->Send();
		free(path);
		delete ex;
	}
}

void _stdcall FreePlugin(std::string name) {
	for (int i = 0; i < plugins->Count(); i++) {
		if (std::get<0>((*plugins)[i]) == name) {
			FreeLibrary(std::get<1>((*plugins)[i]));
			plugins->Delete(i);
			SF->Log("[MOONWALKER]: plugin \"" + name + "\" succesfuly unloaded");
			break;
		}
	}
}

std::string GetFileExt(std::string name) {
	std::string temp;
	bool write_mode = false;
	for (int i = 0; i < name.length(); i++) {
		if (write_mode) temp += name[i];
		if (name[i] == '.' && !write_mode) {
			write_mode = true;
		}
	}
	return temp;
}

void LoadPlugins() {
	char *current_directory = (char*)malloc(512);
	GetCurrentDirectory(512, current_directory);
	char *walker_directory = (char*)malloc(512);
	sprintf(walker_directory, "%s\\WALKER\\*", current_directory);
	SF->Log("[MOONWALKER]: Walker working directory: " + std::string(walker_directory));
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile(walker_directory, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE) {
		do {
			if(std::string(FindFileData.cFileName) != "." && std::string(FindFileData.cFileName) != "..")
				if(GetFileExt(FindFileData.cFileName) == "sf")
					LoadPlugin(FindFileData.cFileName);
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
		SF->Log("[MOONWALKER]: Moonwalker loaded. Version: " + std::string(MOONWALKER_VERSION));
		LoadPlugins();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}