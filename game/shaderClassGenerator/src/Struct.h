#pragma once
#include <string>
#include <vector>
#include "ShaderVariable.h"

class Struct
{

private:
	std::string name;
	std::vector<ShaderVariable> fields;
	
public:
	Struct(const std::string& data, const std::string& namePrefix, const std::vector<std::string>& glslDataTypes);

	inline const std::string& getName() const { return name; }
	inline std::string& getName() { return name; }

	inline const std::vector<ShaderVariable>& getFields() const { return fields; }
	inline std::vector<ShaderVariable>& getFields() { return fields; }
};
