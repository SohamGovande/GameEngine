#include <iostream>
#include "BinaryReader.h"
#include "EndianChecker.h"

BinaryReader::BinaryReader(const std::string& filepath)
	: filepath(filepath), reader(filepath, std::ios::in | std::ios::binary)
{

}

BinaryReader::~BinaryReader()
{
	if (reader.is_open())
		reader.close();
}

void BinaryReader::read(char* block, unsigned int size)
{
	if (GetEndianness() == BIG_ENDIAN)
	{
		for (unsigned int i = 0; i < size; i++)
			reader.read(block + size - i - 1, 1);
	}
	else
	{
		reader.read(block, size);
	}
}

void BinaryReader::ensureHeader(const char* signature, const Version& version)
{
	unsigned int len = strlen(signature);
	char* block = new char[len];
	reader.read(block, len);

	if (memcmp(block, signature, len) == 0) //the strings are equal
	{
		unsigned char major = read<unsigned char>();
		unsigned short minor = read<unsigned short>();
		unsigned char patch = read<unsigned char>();
		if (version != Version(major, minor, patch))
		{
			std::cout << "Binary file " << filepath << " has version " <<
				major << '.' << minor << '.' << patch << "(expected" <<
				version.major << '.' << version.minor << '.' << version.patch << ')' <<
				std::endl;
		}
	}
	else
	{
		std::cout << "Binary file " << filepath << " has signature " <<
			block << "(read " << len << "), but expected" << signature << std::endl;
	}

	delete[] block;
}
