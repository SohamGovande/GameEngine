#pragma once
#include <unordered_map>
#include <string>
#include <vector>

class ElementMap;

class ShaderVariable
{
public:
	std::string name, type, arraySize;

	bool isArray, isStruct;

public:
	ShaderVariable(const std::string& name, const std::string& type);
	ShaderVariable(const std::string& line, const std::string& structNamePrefix, const std::vector<std::string>& glslDataTypes);
	ShaderVariable();

	void append(std::ostream& out, const ElementMap& map, const std::unordered_map<std::string, std::string>& glslCppMap) const;
};
