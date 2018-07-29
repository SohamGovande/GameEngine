#include <algorithm>
#include <cctype>
#include "Struct.h"
#include "StringUtils.h"

Struct::Struct(const std::string& data, const std::string& namePrefix, const std::vector<std::string>& glslDataTypes)
{
	const unsigned int structDeclSize = strlen("struct ");
	const unsigned int openingCurlyBrace = data.find('{');
	const unsigned int closingCurlyBrace = data.find_last_of('}');

	name = namePrefix +  StringUtils::trim(data.substr(structDeclSize, openingCurlyBrace - structDeclSize));

	std::string fieldData = data.substr(openingCurlyBrace+1, closingCurlyBrace - openingCurlyBrace - 1);
	std::vector<std::string> individualFieldData = StringUtils::split(fieldData, ';');
	for (const std::string& fieldData : individualFieldData)
	{
		std::string trimmedFieldData = StringUtils::trim(fieldData);
		if (trimmedFieldData.size() > 0)
		{
			fields.emplace_back(trimmedFieldData + ';', namePrefix, glslDataTypes);
		}
	}
}
