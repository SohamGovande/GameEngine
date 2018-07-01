#pragma once
#include "BinaryWriter.h"
#include "EndianChecker.h"

template<typename T, typename SizeType>
void BinaryWriter::writeBlock(const T* begin, SizeType count)
{
	if (GetEndianness() == BIG_ENDIAN)
		for (SizeType i = 0; i < count; i++)
			write<T>(begin[i]);
	else
		writer.write(reinterpret_cast<const char*>(begin), static_cast<std::streamsize>(sizeof(T) * count));
}