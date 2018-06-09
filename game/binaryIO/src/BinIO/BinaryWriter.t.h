#pragma once
#include "BinaryWriter.h"
#include "EndianChecker.h"

template<typename T, typename U>
void BinaryWriter::writeBlock(const T* begin, U count)
{
	if (GetEndianness() == LITTLE_ENDIAN)
		for (unsigned int i = 0; i < count; i++)
			write<T>(begin[i]);
	else
		writer.write(reinterpret_cast<const char*>(begin), static_cast<std::streamsize>(count));
}