#include <iostream>
#include "BinaryWriter.h"
#include "EndianChecker.h"

BinaryWriter::BinaryWriter(const std::string& filepath)
	: writer(filepath, std::ios::out | std::ios::binary)
{
}

BinaryWriter::~BinaryWriter()
{
	if (writer.is_open())
		writer.close();
}

void BinaryWriter::write(const char* data, unsigned int size)
{
	if (getEndianness() == LITTLE_ENDIAN)
	{
		//Write the data in reverse
		for (unsigned int i = 0; i < size; i++)
			writer.write(&data[size - i - 1], 1);
	}
	else
	{
		//Data is already big endian
		writer.write(data, size);
	}
}

template<typename T, typename U>
void BinaryWriter::writeBlock(const T* begin, U count)
{
	write<U>(count);

	if (getEndianness() == LITTLE_ENDIAN)
		for (unsigned int i = 0; i < count; i++)
			write<T>(begin[i]);
	else
		writer.write(reinterpret_cast<const char*>(begin), static_cast<std::streamsize>(count));
}