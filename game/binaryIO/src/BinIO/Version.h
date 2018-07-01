#pragma once
#include <initializer_list>

class Version
{
public:
	unsigned char major, patch;
	unsigned short minor;

	inline Version(unsigned char major, unsigned short minor, unsigned short patch)
		: major(major), minor(minor), patch(patch)
	{}

	inline bool operator==(const Version& other) const
	{
		return major == other.major && minor == other.minor && patch == other.patch;
	}

	inline bool operator!=(const Version& other) const
	{
		return !operator==(other);
	}
};
