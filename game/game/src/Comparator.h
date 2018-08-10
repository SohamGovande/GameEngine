#pragma once
#include <memory.h>

namespace Comparators
{
	template<typename T>
	inline int compareValues(const T& left, const T& right)
	{
		return ((left < right) ? -1 : (left > right));
	}

	template<>
	inline int compareValues<>(const std::string& left, const std::string& right)
	{
		if (int sizeComparator = compareValues(left.size(), right.size())) return sizeComparator;
		else return memcmp(left.c_str(), right.c_str(), left.size());
	}
}