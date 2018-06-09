#pragma once
#include <array>


constexpr unsigned int Length(const char* str)
{
	return *str ? (1 + Length(str + 1)) : 0;
}

constexpr unsigned int CountUppercase(const char* str, unsigned int strlen)
{
	return strlen == 0 ? 0 : (CountUppercase(str + 1, strlen - 1) + (str[0] >= 'A' && str[0] <= 'Z' ? 1 : 0));
}

template<unsigned int strlen, unsigned int uppercase, unsigned int arraySize = strlen + uppercase + 1>
std::array<char, arraySize> FormatName(const char* str)
{
	std::array<char, arraySize> name;
	unsigned int index = 0;

	for (unsigned int i = 0; i < strlen; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			name[index++] = '_';
			name[index++] = str[i] + ('a' - 'A');
		}
		else
		{
			name[index++] = str[i];
		}
	}
	name[arraySize - 1] = '\0';
	return name;
}
