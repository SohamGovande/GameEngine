#include <array>
#include <vector>
#include <iostream>
#include "Header.h"

struct VecTest
{
	std::vector<int> vec;

	VecTest(std::vector<int>& v)
		: vec(std::move(v))
	{}
};

void Run()
{
	std::vector<int> vec{ 5, 1, 6, 3 };
	VecTest v(vec);
	__debugbreak();
}