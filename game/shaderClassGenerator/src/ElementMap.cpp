#include <iostream>
#include "ElementMap.h"

static void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

ElementMap::ElementMap()
{
}

std::string ElementMap::apply(const std::string& in) const
{
	std::string out = in;
	for (const auto& pair : elements)
		ReplaceAll(out, "${" + pair.first + "}", pair.second.getData());
	return out;
}

void ElementMap::insert(const ElementMap& other)
{
	elements.reserve(elements.size() + other.elements.size());
	for (const auto& elem : other.elements)
		elements.insert_or_assign(std::move(elem.first), std::move(elem.second));
}

const ReplacableElement& ElementMap::requiredFetch(const std::string& key) const
{
	auto it = elements.find(key);
	if (it == elements.end())
	{
		std::cout << "Error! No value found: \'" << key << '\'' << std::endl;
		ReplacableElement* null = nullptr;
		return *null;
	}
	else
	{
		return it->second;
	}
}

const std::string& ElementMap::operator[](const std::string& key) const
{
	return requiredFetch(key).getData();
}

