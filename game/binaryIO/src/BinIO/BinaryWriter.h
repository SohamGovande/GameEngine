#pragma once
#include <fstream>
#include <string>

class BinaryWriter
{
private:
	std::ofstream writer;

public:
	BinaryWriter(const std::string& filepath);
	~BinaryWriter();

	void write(const char* data, unsigned int size);

	template<typename T> //Primitive / fundamental type
	inline void write(T value) //T is a primitive, so don't pass it by const reference
	{
		write(reinterpret_cast<const char*>(&value), sizeof(T)); 
	}
	
	//Optimization for 1-byte types
	template<>
	inline void write<char>(char value)
	{
		writer.write(&value, 1);
	}

	template<>
	inline void write<unsigned char>(unsigned char value)
	{
		writer.write(reinterpret_cast<const char*>(&value), 1);
	}

	inline unsigned int getWritePos() { return writer.tellp(); }
	inline void close() { writer.close(); }
	inline std::ofstream& getWriter() { return writer; }
};
