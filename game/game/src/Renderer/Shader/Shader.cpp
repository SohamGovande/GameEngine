#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "../GlMacros.h"
#include "Shader.h"

#define SHADERS_BASE_DIR "res/shader/shaders/"
#define SHADERS_INCLUDE_DIR "res/shader/include/"

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile,
	std::vector<ShaderPreprocessorElement>&& vPreprocessors,
	std::vector<ShaderPreprocessorElement>&& fPreprocessors)

	: rendererID(0), vs(0), fs(0),
	vertexFile(SHADERS_BASE_DIR + vertexFile), fragmentFile(SHADERS_BASE_DIR + fragmentFile),
	vPreprocessorElements(std::move(vPreprocessors)), fPreprocessorElements(std::move(fPreprocessors))
{
	rendererID = createShader(
		readFile(this->vertexFile, vPreprocessorElements, GL_VERTEX_SHADER),
		readFile(this->fragmentFile, fPreprocessorElements, GL_FRAGMENT_SHADER)
	);

	//Find which directory the shader files are located in.
	//NOTE: Should be the same for both vertex and fragment shaders.
	unsigned int baseShadersDirLen = strlen(SHADERS_BASE_DIR);
	unsigned int specificShaderDirIndex = this->vertexFile.find('/', baseShadersDirLen) + 1;
	shaderDirectory = this->vertexFile.substr(baseShadersDirLen, specificShaderDirIndex - baseShadersDirLen);
}

Shader::Shader(Shader&& other)
	: rendererID(other.rendererID), vs(other.vs), fs(other.fs),
	
	uniformLocationCache(std::move(other.uniformLocationCache)),
	vPreprocessorElements(std::move(other.vPreprocessorElements)),
	fPreprocessorElements(std::move(other.fPreprocessorElements)),
	
	vertexFile(std::move(other.vertexFile)), fragmentFile(std::move(other.fragmentFile)),
	shaderDirectory(std::move(other.shaderDirectory)),

	includedFiles(std::move(other.includedFiles))
{
	other.rendererID = other.vs = other.fs = 0;
}

Shader::~Shader()
{
	release();
}

Shader& Shader::operator=(Shader&& other)
{
	if (this != &other)
	{
		release();

		rendererID = other.rendererID;
		vs = other.vs;
		fs = other.fs;
		other.rendererID = other.vs = other.fs = 0;

		uniformLocationCache = std::move(other.uniformLocationCache);
		vPreprocessorElements = std::move(other.vPreprocessorElements);
		fPreprocessorElements = std::move(other.fPreprocessorElements);
		
		vertexFile = std::move(other.vertexFile);
		fragmentFile = std::move(other.fragmentFile);
		shaderDirectory = std::move(other.shaderDirectory);
		
		includedFiles = std::move(other.includedFiles);
	}
	return *this;
}

void Shader::release()
{
	GlCall(glDetachShader(rendererID, vs));
	GlCall(glDetachShader(rendererID, fs));
	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));
	GlCall(glDeleteProgram(rendererID));
}

int Shader::getUniformLocation(const std::string& name)
{
	auto it = uniformLocationCache.find(name);
	if (it != uniformLocationCache.end())
		return it->second;

	GlCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform " << name << " doesn't exist." << std::endl;

	uniformLocationCache.emplace(name, location);
	return location;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GlCall(GLuint program = glCreateProgram());
	vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));

	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	return program;
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type)
{
	GlCall(unsigned int shaderId = glCreateShader(type));
	const char* src = source.c_str();
	GlCall(glShaderSource(shaderId, 1, &src, nullptr));
	GlCall(glCompileShader(shaderId));

	int result;
	GlCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int len;
		GlCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len));
		char* message = new char[len];

		bool vertexShader = type == GL_VERTEX_SHADER;

		GlCall(glGetShaderInfoLog(shaderId, len, &len, message));

		std::cout << (vertexShader ? "VERTEX" : "FRAGMENT") <<
			" shader compilation error: " <<
			(vertexShader ? vertexFile : fragmentFile) << std::endl;
		std::cout << message << std::endl;

		GlCall(glDeleteShader(0));

		delete[] message;

		//Allow the shader to be recompiled on the fly
		std::cout << "Press ENTER to recompile! ";
		std::cin.get();

		includedFiles.clear();
		std::string newSource = readFile(
			vertexShader ? vertexFile : fragmentFile,
			vertexShader ? vPreprocessorElements : fPreprocessorElements,
			type
		);

		return compileShader(newSource, type);
	}

	return shaderId;
}

std::string Shader::readFile(const std::string& filename, const std::vector<ShaderPreprocessorElement>& preprocessor, unsigned int shaderType)
{
	std::stringstream ss;
	std::ifstream stream(filename);
	if (!stream.is_open())
	{
		std::cout << "Unable to open shader file " << filename << std::endl;
		return std::string();
	}

	std::string line;
	
	while (getline(stream, line))
	{
		if (line.find("#include") == 0)
		{
			const unsigned int includeDirectiveSize = strlen("#include ");
			std::string afterInclude = line.substr(includeDirectiveSize, line.size() - includeDirectiveSize);
			std::string includeDir;
			if (line[includeDirectiveSize] == '<')
			{
				//Include the file from res/shaders/include/
				includeDir = SHADERS_INCLUDE_DIR;
			}
			else if (line[includeDirectiveSize] == '\"')
			{
				//Include the file from the current shader's directory
				includeDir = shaderDirectory;
			}
			std::pair<std::unordered_set<std::string>::iterator, bool> positionInSet = includedFiles.insert(includeDir + afterInclude.substr(1, afterInclude.size() - 2));
			if (positionInSet.second) { //The file has not already been included
				const std::string& includeFilepath = *positionInSet.first;
				std::string filedata = readFile(includeFilepath, preprocessor, shaderType);
				ss << filedata << '\n';
			}
		}
		else
		{
			ss << line << '\n';
		}

		if (line.find("#version") == 0)
		{
			for (const ShaderPreprocessorElement& preproc : preprocessor)
				ss << "#define " << preproc.name << " " << preproc.value << '\n';
		}
	}
	
	return ss.str();
}


void Shader::bind() const
{
	GlCall(glUseProgram(rendererID));
}

void Shader::unbind() const
{
	GlCall(glUseProgram(0));
}
