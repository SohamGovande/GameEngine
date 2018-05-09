#pragma once
#include <string>

struct ShaderPreprocessorElement
{
	std::string name, value;

	inline ShaderPreprocessorElement(const std::string& name, const std::string& value)
		: name(name), value(value)
	{}
};