#include <cctype>
#include "StringUtils.h"

std::string StringUtils::trim(const std::string& str)
{
	unsigned int beginSpaces = 0;
	unsigned int endSpaces = 0;

	for (unsigned int& i = beginSpaces; i != str.size() && std::isspace(str[i]); i++);
	for (unsigned int& i = endSpaces;	i != str.size() && std::isspace(str[str.size() - i - 1]); i++);

	//Allow for "trimming" empty strings
	if (beginSpaces + endSpaces > str.size())
		endSpaces = 0;

	return str.substr(beginSpaces, str.size() - beginSpaces - endSpaces);
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimeter)
{
	std::vector<std::string> split;
	std::string buf;

	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (str[i] == delimeter)
		{
			if (!buf.empty())
				split.emplace_back(std::move(buf));
		}
		else
		{
			buf.push_back(str[i]);
		}
	}
	if (!buf.empty())
		split.emplace_back(std::move(buf));

	return split;
}
