
#pragma once
#include "BinaryReader.h"
#include "EndianChecker.h"

template<typename T, typename SizeType>
T* BinaryReader::readBlock(SizeType count)
{
	const unsigned int size = sizeof(T) * count;
	T* block = reinterpret_cast<T*>(malloc(size));

	if (getEndianness() == LITTLE_ENDIAN)
	{
		for (SizeType i = 0; i < count; i++)
			block[i] = read<T>();
	}
	else
	{
		reader.read(reinterpret_cast<char*>(block), size);
	}

	return block;
}