#include <fstream>
#include <iostream>
#include <sstream>
#include "IOHelper.h"

std::string IOHelper::readFile(const std::string& path)
{
	std::stringstream ss;
	std::ifstream input(path);
	if (!input.is_open())
	{
		std::cout << "Error: Could not open file " << path << std::endl;
		return std::string();
	}

	std::string line;
	while (getline(input, line))
		ss << line << '\n';
	input.close();
	return ss.str();
}
