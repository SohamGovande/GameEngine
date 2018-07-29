#pragma once
#include <vector>
#include <string>

namespace StringUtils
{
	std::string trim(const std::string& str);
	std::vector<std::string> split(const std::string& str, char delimter);

}