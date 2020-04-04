#pragma once
#include "walker.h"


enum Mode {
	M_OPEN_WRITE_APPEND = std::ios_base::app,
	M_OPEN_READ = std::ios_base::in,
	M_OPEN_WRITE = std::ios_base::out,
	M_OPEN_WRITE_ATE = std::ios_base::ate,
	M_TRUNC = std::ios_base::trunc,
	M_BINARY = std::ios_base::binary
};

class FileWriter {
public:
	static void Delete(std::string path) {
		remove(path.c_str());
	}
	void Open(std::string path, Mode mode) {
		buff.open(path, mode);
	}
	void Close() {
		buff.close();
	}
	void Write(std::string message) {
		buff << message;
	}
	template <typename T>
	void Write(const T& var) {
		buff << var;
	}
private:
	std::ofstream buff;
};

class FileReader {
public:
	void Open(std::string path, Mode mode = Mode::M_OPEN_WRITE_APPEND) {
		buff.open(path, mode);
	}
	void Close() {
		buff.close();
	}
	template <typename T>
	T Read() const {
		T ret;
		buff >> ret;
		return ret;
	}
	/// <param name = "outbuff"> Out buffer. </param>
	/// <param name = "length"> If length is -1, function will return all text from file. </param>
	void ReadText(char *outbuff, int length = -1) {
		std::string out;
		if (length == -1) {
			while (!buff.eof()) {
				char one_char = '\0';
				buff >> one_char;
				out += one_char;
			}
		}
		else {
			for (int i = 0; i < length; i++) {
				char one_char = '\0';
				buff >> one_char;
				out += one_char;
			}
		}
		strcpy(outbuff, out.c_str());
	}
private:
	std::ifstream buff;
};