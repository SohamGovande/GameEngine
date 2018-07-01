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

void BinaryWriter::writeHeader(const char* header, const Version& version)
{
	writer.write(header, strlen(header));
	write<unsigned char>(version.major);
	write<unsigned short>(version.minor);
	write<unsigned char>(version.patch);
}

void BinaryWriter::write(const char* data, unsigned int size)
{
	if (GetEndianness() == BIG_ENDIAN)
	{
		//Write the data in reverse
		for (unsigned int i = 0; i < size; i++)
			writer.write(data + size - i - 1, 1);
	}
	else
	{
		//Data is already little endian
		writer.write(data, size);
	}
}
