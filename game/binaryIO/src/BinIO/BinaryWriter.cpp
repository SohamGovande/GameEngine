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
	if (GetEndianness() == LITTLE_ENDIAN)
	{
		//Write the data in reverse
		for (unsigned int i = 0; i < size; i++)
			writer.write(data + size - i - 1, 1);
	}
	else
	{
		//Data is already big endian
		writer.write(data, size);
	}
}
