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

	template<typename T, typename U>
	void writeBlock(const T* begin, U count);

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

	//Writing strings
	inline void writeString(const std::string& value)
	{
		write<unsigned short>(value.size());
		writer.write(&*value.begin(), value.size());
	}

	template<>
	inline void write<std::string>(std::string value) = delete; //Delete this because we don't want to copy the parameter

	template<>
	inline void write<const char*>(const char* value)
	{
		writeString(std::string(value));
	}
	
	template<>
	inline void write<char*>(char* value)
	{
		write<const char*>(value);
	}

	inline std::streampos getWritePos() { return writer.tellp(); }
	inline void close() { writer.close(); }
	inline std::ofstream& getWriter() { return writer; }
};
