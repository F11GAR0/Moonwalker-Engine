#pragma once
#include "walker.h"

class Convert {
public:
	template <typename T>
	static std::string ToString(T value) {
		std::string ret;
		ret += value;
		return ret;
	}
	static std::string ToString(unsigned char *value) {
		std::string ret;
		ret += (char*)value;
		return ret;
	}
	static std::string ToString(WORD value) {
		std::string ret;
		char *buff = (char*)malloc(sizeof(WORD) * 64);
		sprintf_s(buff, sizeof(WORD) * 64, "%d", value);
		ret += buff; // i know i know... tss.. dont talk!
		free(buff);
		return ret;
	}
	static int ToInteger(std::string str) {
		return stoi(str);
	}
	static float ToFloat(std::string str) {
		return stof(str);
	}
};