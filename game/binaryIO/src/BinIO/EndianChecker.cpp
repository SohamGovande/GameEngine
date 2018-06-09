#include "EndianChecker.h"

static int endianness = -1;

static void checkEndianness()
{
	short int word = 0x0001;
	char* c = reinterpret_cast<char*>(&word);
	endianness = (c[0] == 1) ? LITTLE_ENDIAN : BIG_ENDIAN;
}

int GetEndianness()
{
	if (endianness == -1)
		checkEndianness();
	return endianness;
}