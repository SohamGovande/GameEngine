#include <algorithm>
#include <iostream>
#include "ShaderFileInfo.h"

void ShaderFile::removeNonUniformStructs()
{
	std::vector<std::vector<Struct>::const_iterator> structsToErase;
	//Assume all structs are going to be erased, and later, mark out the ones that are used for uniforms
	structsToErase.reserve(structs.size());
	for (auto it = structs.cbegin(); it != structs.cend(); it++)
		structsToErase.push_back(it);

	recursivelyRemove(uniforms, structsToErase);

	for (auto structIterator : structsToErase)
		structs.erase(structIterator);
}

void ShaderFile::recursivelyRemove(const std::vector<ShaderVariable>& vars, std::vector<std::vector<Struct>::const_iterator>& structsToErase)
{
	for (const ShaderVariable& var : vars)
	{
		if (var.isStruct)
		{
			//Mark out the target struct for deletion
			std::vector<Struct>::const_iterator structIterator = getStructIt(var.type);
			if (structIterator == structs.cend())
			{
				std::cout << "Error: Unable to find struct " << var.type << "!" << std::endl;
				return;
			}
			//Work on nested structs too
			recursivelyRemove(structIterator->getFields(), structsToErase);

			for (auto it = structsToErase.cbegin(); it != structsToErase.cend(); it++)
			{

				if (structIterator == *it)
				{
					structsToErase.erase(it);
					break;
				}
			}
		}
	}
}

Struct* ShaderFile::getStruct(const std::string& name)
{
	for (Struct& structure : structs)
		if (structure.getName() == name)
			return &structure;
	return nullptr;
}

const Struct* ShaderFile::getStruct(const std::string& name) const
{
	for (const Struct& structure : structs)
		if (structure.getName() == name)
			return &structure;
	return nullptr;
}

std::vector<Struct>::const_iterator ShaderFile::getStructIt(const std::string& name) const
{
	for (auto it = structs.cbegin(); it != structs.cend(); it++)
		if (it->getName() == name)
			return it;
	return structs.cend();
}

std::vector<Struct>::iterator ShaderFile::getStructIt(const std::string& name)
{
	for (auto it = structs.begin(); it != structs.end(); it++)
		if (it->getName() == name)
			return it;
	return structs.end();
}
