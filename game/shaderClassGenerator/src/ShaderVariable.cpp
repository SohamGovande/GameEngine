#include <iostream>
#include "ShaderVariable.h"
#include "ElementMap.h"

ShaderVariable::ShaderVariable(const std::string& name, const std::string& type)
	: name(name), type(type), isArray(false), isStruct(false)
{
}

ShaderVariable::ShaderVariable(const std::string& line, const std::string& structNamePrefix, const std::vector<std::string>& glslDataTypes)
	: isArray(false), isStruct(false)
{
	int spaceIndex = 0;
	int secondSpaceIndex = line.find(' ', spaceIndex) + 1;
	type = line.substr(spaceIndex, secondSpaceIndex - spaceIndex - 1);
	name = line.substr(secondSpaceIndex, line.size() - secondSpaceIndex - 1);

	unsigned int squareBracketPos = name.find('[');
	isArray = (squareBracketPos != line.npos);

	if (isArray)
	{
		arraySize = name.substr(squareBracketPos + 1, name.length() - squareBracketPos - 2);
		name = name.substr(0, squareBracketPos);
	}

	isStruct = std::find(glslDataTypes.begin(), glslDataTypes.end(), type) == glslDataTypes.end();
	if (isStruct)
		type.insert(0, structNamePrefix);
}

ShaderVariable::ShaderVariable()
	: isArray(false), isStruct(false)
{
}

void ShaderVariable::append(std::ostream& out, const ElementMap& map, const std::unordered_map<std::string, std::string>& glslCppMap) const
{
	if (isArray)
		out << "std::array<";

	if (isStruct)
		out << type;
	else
		out << "Uniform<" << glslCppMap.find(type)->second << ">";

	if (isArray)
		out << ", " << arraySize << '>';
	out << ' ' << name << ";\n";
}

