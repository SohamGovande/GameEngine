#pragma once
#include <string>

struct ShaderType
{
	std::string attribName, shaderName, prefix;

	inline ShaderType(const std::string& name, const std::string& shaderName, const std::string& prefix)
		: attribName(name), shaderName(shaderName), prefix(prefix)
	{}
};

