#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Version.h"

class BinaryReader
{
private:
	std::ifstream reader;
	std::string filepath;

public:
	BinaryReader(const std::string& filepath);
	~BinaryReader();

	void read(char* block, unsigned int size);
	void ensureHeader(const char* signature, const Version& version);

	template<typename T, typename SizeType>
	T* readBlock(SizeType count);

	template<typename T>
	inline T read()
	{
		char data[sizeof(T)];
		read(data, sizeof(T));
		return *reinterpret_cast<T*>(data);
	}

	inline std::string readString()
	{
		unsigned short len = read<unsigned short>();
		std::string str;
		str.reserve(len);
		reader.read(&str.front(), len);
		return str;
	}

	inline std::streamoff getReadPos() { return reader.tellg(); }
	inline void close() { reader.close(); }
	inline std::ifstream& getReader() { return reader; }
};
