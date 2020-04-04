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
	static int ToInteger(std::string str) {
		return stoi(str);
	}
	static float ToFloat(std::string str) {
		return stof(str);
	}
};