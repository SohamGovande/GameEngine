#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <cctype>
#include <string>
#include <vector>

#include <cxxopts.hpp>

#include "ReplacableElement.h"
#include "ElementMap.h"
#include "ShaderFileInfo.h"
#include "ShaderType.h"
#include "StringUtils.h"

#define TEST_ENVIRONMENT true

ElementMap ProcessFile(const std::string& filename)
{
	ElementMap elementMap;
	ElementMap::MapType& map = elementMap.getUnderlyingMap();

	std::pair<std::string, ReplacableElement> nextPair;

	std::ifstream filestream(filename);
	if (!filestream.is_open())
	{
		std::cout << "Error: Unable to open file " << filename << std::endl;
		return elementMap;
	}
	std::string line;
	bool firstPair = true;

	while (getline(filestream, line))
	{
		if (line[0] == '-')
		{
			if (!firstPair)
			{
				nextPair.second.assign(StringUtils::trim(nextPair.second.getData()));
				map.insert(std::move(nextPair));
			}
			else
			{
				firstPair = false;
			}

			nextPair.first = std::move(line.substr(1, line.size() - 1));
		}
		else
		{
			std::string& value = nextPair.second.getData();
			value.append(line);
			value.append("\n");
		}
	}

	if (!nextPair.first.empty())
	{
		nextPair.second.assign(StringUtils::trim(nextPair.second.getData()));
		map.insert(std::move(nextPair));
	}

	filestream.close();

	return elementMap;
}

std::string ReadRawFile(const std::string& filename)
{
	std::ifstream fs(filename);
	if (!fs.is_open())
	{
		std::cout << "Error: Unable to read file " << filename << std::endl;
		return std::string();
	}

	std::stringstream ss;
	std::string line;

	while (getline(fs, line))
		ss << line << '\n';

	fs.close();
	return ss.str();
}

ShaderFile ParseShaderFile(const std::string& filename, const std::string& structNamePrefix, const std::vector<std::string>& glslDataTypes)
{
	ShaderFile shader;

	std::string line;
	std::string filepath = "X:/dev/cpp/game/game/game/res/shader/shaders/" + filename;
	std::ifstream fs(filepath);

	if (!fs.is_open())
	{
		std::cout << "Error: Unable to open shader file " << filepath << std::endl;
		return shader;
	}

	std::string tempStructData;
	bool isReadingStruct = false;

	while (getline(fs, line))
	{
		if (line.find("uniform") == 0)
		{
			const unsigned int uniformDeclSize = strlen("uniform ");
			shader.getUniforms().emplace_back(
				line.substr(uniformDeclSize, line.size() - uniformDeclSize), structNamePrefix, glslDataTypes
			);
		}
		else if (line.find("struct") == 0)
		{
			isReadingStruct = true;
		}

		if (isReadingStruct)
		{
			tempStructData.append(line).append("\n");

			//Finish reading the struct and send it off to be parsed
			if (line.find('}') != line.npos) 
			{
				isReadingStruct = false;
				shader.getStructs().emplace_back(tempStructData, structNamePrefix, glslDataTypes);
				tempStructData = "";
			}
		}
	}

	fs.close();
	return shader;
}

std::string GetShaderUniformDeclarations(
	const ElementMap& map,
	const ShaderFile& shader,
	const std::string& structNamePrefix,
	const std::unordered_map<std::string, std::string>& glslCppMap
)
{
	std::stringstream out;
	for (const ShaderVariable& uni : shader.getUniforms())
	{
		out << '\t';
		uni.append(out, map, glslCppMap);
	}
	return out.str();
}

std::string CreateAllUniformDeclarations(
	ElementMap& map,
	const std::vector<std::pair<ShaderType, ShaderFile>>& shaders,
	const std::unordered_map<std::string, std::string>& glslCppMap
)
{
	std::stringstream out;

	for (const auto& shaderPair : shaders)
	{
		const ShaderType& type = shaderPair.first;
		const ShaderFile& shader = shaderPair.second;
		
		if (shader.getUniforms().size() > 0)
		{
			out << "\t//" << type.shaderName << " shader uniforms\n";
			out << GetShaderUniformDeclarations(map, shader, type.prefix, glslCppMap);
			out << '\n';
		}
	}

	return out.str();
}

void GenerateRecursiveShaderUniformInitializer(
	std::ostream& out,
	const ElementMap& map,
	const std::string& uniNamePrefix,
	const std::string& uniNameStringPrefix,
	const std::string& linePrefix,
	char nextForLoopVar,
	const ShaderFile& shader,
	const ShaderVariable& uni
)
{
	if (uni.isStruct)
	{
		const Struct* structure = shader.getStruct(uni.type);
		if (structure == nullptr)
		{
			std::cout << "Error: No structure " << uni.type << " found!" << std::endl;
			return;
		}

		out << linePrefix;
		if (uni.isArray)
		{
			out << "for(unsigned int " << nextForLoopVar << " = 0; " << nextForLoopVar << " < (" << uni.arraySize << "); " << nextForLoopVar << "++)\n"
				<< linePrefix << "{\n";
			for (const ShaderVariable& field : structure->getFields())
			{
				GenerateRecursiveShaderUniformInitializer(
					out,
					map,
					uniNamePrefix + uni.name + "[" + nextForLoopVar + "].", 
					uniNameStringPrefix + uni.name + "[\" + std::to_string(" + nextForLoopVar + ") + \"].", 
					"\t" + linePrefix,
					nextForLoopVar + 1,
					shader,
					field
				);
			}
			out << linePrefix << "}\n";
		}
		else
		{
			for (const ShaderVariable& field : structure->getFields())
				GenerateRecursiveShaderUniformInitializer(
					out,
					map,
					uniNamePrefix + uni.name + ".",
					uniNameStringPrefix + uni.name + ".",
					linePrefix,
					nextForLoopVar,
					shader,
					field
				);
		}
	}
	else
	{
		out << linePrefix;
		std::string uniAccessor = uniNamePrefix + uni.name;
		std::string uniStringAccessor = uniNameStringPrefix + uni.name;
		if (uni.isArray)
		{
			out << "for(unsigned int " << nextForLoopVar << " = 0; " << nextForLoopVar << " < (" << uni.arraySize << "); " << nextForLoopVar << "++)\n";
			out << linePrefix << "\t" << uniAccessor << "[" << nextForLoopVar << "]" << ".loadLocation(\"" 
				<< uniStringAccessor << "[\" + std::to_string(" << nextForLoopVar << ") + \"]\""
				", programID);\n";
		}
		else
		{
			out << uniAccessor << ".loadLocation(\"" << uniStringAccessor << "\", programID);\n";
		}
	}
}

std::string GetShaderUniformInitializers(const ElementMap& map, const ShaderFile& shader)
{
	std::stringstream ss;
	
	for (const ShaderVariable& uni : shader.getUniforms())
	{
		GenerateRecursiveShaderUniformInitializer(ss, map, "", "", "\t\t", 'i', shader, uni);
	}
	return ss.str();
}

std::string CreateAllUniformInitializers(
	ElementMap& map,
	const std::vector<std::pair<ShaderType, ShaderFile>>& shaders
)
{
	std::stringstream out;
	out << "\t\t//We need the GL program ID to load uniform locations for that program.\n";
	out << "\t\tconst GLuint programID = " << map["GetShaderProgramIDFunction"] << ";\n\n";

	for (const auto& shaderPair : shaders)
	{
		const ShaderType& type = shaderPair.first;
		const ShaderFile& shader = shaderPair.second;
		if (shader.getUniforms().size() > 0)
		{
			out << "\t\t//" << shaderPair.first.shaderName << " shader uniforms\n";
			out << GetShaderUniformInitializers(map, shaderPair.second);
			out << '\n';
		}
	}

	return out.str();
}

std::string GetShaderStructDeclarations(
	const ElementMap& map,
	const ShaderFile& shader,
	const std::string& structNamePrefix,
	const std::unordered_map<std::string, std::string>& glslCppMap
)
{
	std::stringstream out;

	if (shader.getStructs().size() > 0)
	{
		out <<	"public:\n";

		for (const Struct& structure : shader.getStructs())
		{
			out << "\tstruct " << structure.getName() << "\n\t{\n";
			for (const ShaderVariable& field : structure.getFields())
			{
				out << "\t\t";
				field.append(out, map, glslCppMap);
			}
			out << "\t};\n";
		}
	}

	return out.str();
}

std::string CreateAllUniformStructDeclarations(
	const ElementMap& map,
	const std::vector<std::pair<ShaderType, ShaderFile>>& shaders,
	const std::unordered_map<std::string, std::string>& glslCppMap
)
{
	std::stringstream out;
	for (const auto& shaderPair : shaders)
	{
		const ShaderType& type = shaderPair.first;
		const ShaderFile& shader = shaderPair.second;

		if (shader.getStructs().size() > 0)
		{
			out << "//" << shaderPair.first.shaderName << " uniform structs\n";
			out << GetShaderStructDeclarations(map, shaderPair.second, shaderPair.first.prefix, glslCppMap);
			out << '\n';
		}
	}

	return out.str();
}

ElementMap LoadShaderReplacables(
	const ElementMap& userMap,
	const ElementMap& config,
	const std::vector<ShaderType>& shaderTypes,
	const std::unordered_map<std::string, std::string> glslCppMap,
	const std::vector<std::string>& glslPrimitiveTypes
)
{
	ElementMap elementMap;
	ElementMap::MapType& map = elementMap.getUnderlyingMap();

	//Copy certain properties from the configuration file
	map.emplace("GetShaderProgramIDFunction", config["GetShaderProgramIDFunction"]);

	std::vector<std::pair<ShaderType, ShaderFile>> shaders;

	for (const ShaderType& shader : shaderTypes)
	{
		auto shaderFilepathIt = userMap.getUnderlyingMap().find(shader.attribName);
		if (shaderFilepathIt == userMap.getUnderlyingMap().end())
		{
			//The shader stage wasn't specified in the shader-gen file
			continue;
		}

		shaders.emplace_back(shader, ParseShaderFile(shaderFilepathIt->second.getData(), shader.prefix, glslPrimitiveTypes));
		ShaderFile& shaderInfo = shaders.back().second;

		//Remove all structs that aren't used by uniforms
		shaderInfo.removeNonUniformStructs();
	}



	//The actual things in the generated shaderfile
	map.emplace("Structs",				CreateAllUniformStructDeclarations(elementMap, shaders, glslCppMap));
	map.emplace("Uniforms",				CreateAllUniformDeclarations(elementMap, shaders, glslCppMap));
	map.emplace("LoadUniformLocations", CreateAllUniformInitializers(elementMap, shaders));
	return elementMap;
}

void LoadGlslTypeInformation(ElementMap& config, std::vector<std::string>& glslDataTypes, std::unordered_map<std::string, std::string>& glslCppMap)
{
	for (const std::string& line : StringUtils::split(config["GlslDataTypes"], '\n'))
	{
		size_t spaceIndex = line.find(' ');
		std::string glslName = line.substr(0, spaceIndex);
		std::string cppName = line.substr(spaceIndex + 1);

		glslDataTypes.push_back(glslName);
		glslCppMap.emplace(std::move(glslName), std::move(cppName));
	}
}

void LoadShaderTypeInformation(ElementMap& config, std::vector<ShaderType>& shaderTypes)
{
	for (const std::string& line : StringUtils::split(config["SpecificShaderInfo"], '\n'))
	{
		size_t spaceIndex = line.find(' ');
		size_t lastSpaceIndex = line.find_last_of(' ');
		shaderTypes.emplace_back(
			line.substr(0, spaceIndex),
			line.substr(spaceIndex + 1, lastSpaceIndex - spaceIndex - 1),
			line.substr(lastSpaceIndex + 1)
		);
	}
}

int main(int argc, char* argv[])
{
	cxxopts::Options options(argv[0], "ShaderCppGen Help");
	options.add_options()
		("c,config", "Config file", cxxopts::value<std::string>()->default_value("Config.txt"), "(Optional - override the configuration file Config.txt)")
		("t,template", "Template class file", cxxopts::value<std::string>()->default_value("Template.txt"), "(Optional - override the template class file Template.txt)")
		("i,input", "Input files (usage: \"-i file1 -i file2...\")", cxxopts::value<std::vector<std::string>>(), "(Required - the files to process and generate headers from)");

	cxxopts::ParseResult result = options.parse(argc, argv);

	const cxxopts::OptionValue& configFileOption = result["config"];
	const cxxopts::OptionValue& templateFileOption = result["template"];
	const cxxopts::OptionValue& inputFilesOption = result["input"];

	if (inputFilesOption.count() == 0)
	{
		std::cout << options.help() << std::endl;
		if (TEST_ENVIRONMENT)
			std::cin.get();
		return 1;
	}

	std::string configFile = configFileOption.as<std::string>();
	std::string templateFile = templateFileOption.as<std::string>();
	std::vector<std::string> files = inputFilesOption.as<std::vector<std::string>>();

	ElementMap config = ProcessFile(configFile);
	
	//Load GLSL type information
	std::vector<std::string> glslDataTypes;
	std::unordered_map<std::string, std::string> glslCppMap;
	LoadGlslTypeInformation(config, glslDataTypes, glslCppMap);

	//Load shader type information
	std::vector<ShaderType> shaderTypes;
	LoadShaderTypeInformation(config, shaderTypes);

	//Load the template file
	std::string templateData = ReadRawFile(templateFile);

	for (const std::string& shgenFile : files)
	{
		ElementMap shaderGen = ProcessFile(shgenFile);
		ElementMap shaderProperties = LoadShaderReplacables(shaderGen, config, shaderTypes, glslCppMap, glslDataTypes);
		shaderProperties.insert(shaderGen);

		std::string evaluated = shaderProperties.apply(templateData);

		std::string filepath = config["OutputParentDir"] + shaderProperties["ClassName"] + '.' + config["FileExtension"];
		std::ofstream fs(filepath);
		if (!fs.is_open())
		{
			std::cout << "Error: Unable to write to file " << filepath << std::endl;
		}
		else
		{
			fs.write(evaluated.c_str(), evaluated.size());
			fs.close();

			std::cout << "Generated " << filepath << " from " << shgenFile << std::endl;
		}
		
	}

	if (TEST_ENVIRONMENT)
		std::cin.get();

	return 0;
}
