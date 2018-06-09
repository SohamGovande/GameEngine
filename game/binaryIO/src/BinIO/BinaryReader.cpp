#include "BinaryReader.h"
#include "EndianChecker.h"

BinaryReader::BinaryReader(const std::string& filepath)
	: reader(filepath, std::ios::in | std::ios::binary)
{

}

BinaryReader::~BinaryReader()
{
	if (reader.is_open())
		reader.close();
}

void BinaryReader::read(char* block, unsigned int size)
{
	if (GetEndianness() == LITTLE_ENDIAN)
	{
		for (unsigned int i = 0; i < size; i++)
			reader.read(&block[size - i - 1], 1);
	}
	else
	{
		reader.read(block, size);
	}
}
