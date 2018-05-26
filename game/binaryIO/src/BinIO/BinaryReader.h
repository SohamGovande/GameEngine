#pragma once
#include <fstream>
#include <string>

class BinaryReader
{
private:
	std::ifstream reader;

public:
	BinaryReader(const std::string& filepath);
	~BinaryReader();

	void read(char* block, unsigned int size);

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