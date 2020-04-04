#pragma once
#include "walker.h"

#define BASE_EX_LENGTH 128

class Exception {
public:
	Exception(const char *classname, DWORD addr, const char *func_name, int code, const char *message) {
		this->message = (char*)malloc(BASE_EX_LENGTH + strlen(classname) + strlen(func_name) + strlen(message));
		sprintf(this->message, "[MOONWALKER]: Exception[C%d] in class {%s}[0x%X], at function {%s}. Message: %s", code, classname, addr, func_name, message);
		loaded = true;
	}
	Exception(const char *func_name, int code, const char *message) {
		this->message = (char*)malloc(BASE_EX_LENGTH + strlen(func_name) + strlen(message));
		sprintf(this->message, "[MOONWALKER]: Exception[C%d] at function {%s}. Message: %s", code, func_name, message);
		loaded = true;
	}
	~Exception() {
		free(message);
	}
	void Send() {
		if(loaded)
			NJIN->Log(this->message);
	}
private:
	bool loaded;
	char *message;
};