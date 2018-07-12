#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "../GlMacros.h"
#include "Shader.h"

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile,
	std::vector<ShaderPreprocessorElement>&& vPreprocessors,
	std::vector<ShaderPreprocessorElement>&& fPreprocessors)
	: rendererID(0), vs(0), fs(0),
	vertexFile("res/shader/shaders/" + vertexFile), fragmentFile("res/shader/shaders/" + fragmentFile),
	vPreprocessorElements(std::move(vPreprocessors)), fPreprocessorElements(std::move(fPreprocessors))
{
	rendererID = createShader(
		readFile(this->vertexFile, vPreprocessorElements),
		readFile(this->fragmentFile, fPreprocessorElements)
	);

	//Find which directory the shader files are located in.
	//NOTE: Should be the same for both vertex and fragment shaders.
	unsigned int baseShadersDirLen = strlen("res/shader/shaders/");
	unsigned int specificShaderDirIndex = this->vertexFile.find('/', baseShadersDirLen) + 1;
	shaderDirectory = this->vertexFile.substr(baseShadersDirLen, specificShaderDirIndex - baseShadersDirLen);
}

Shader::Shader(Shader&& other)
	: rendererID(other.rendererID), vs(other.vs), fs(other.fs),
	
	uniformLocationCache(std::move(other.uniformLocationCache)),
	vPreprocessorElements(std::move(other.vPreprocessorElements)),
	fPreprocessorElements(std::move(other.fPreprocessorElements)),
	
	vertexFile(std::move(other.vertexFile)), fragmentFile(std::move(other.fragmentFile))
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

		vPreprocessorElements = std::move(other.vPreprocessorElements);
		fPreprocessorElements = std::move(other.fPreprocessorElements);
		rendererID = other.rendererID;
		vs = other.vs;
		fs = other.fs;
		other.rendererID = other.vs = other.fs = 0;
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
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	GlCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform " << name << " doesn't exist." << std::endl;

	uniformLocationCache[name] = location;
	return location;
}

void Shader::setVec4(const std::string& name, float v0, float v1, float v2, float v3)
{
	GlCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setVec4(const std::string & name, const glm::vec4 & vec)
{
	GlCall(glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
}

void Shader::setFloat(const std::string& name, float v)
{
	GlCall(glUniform1f(getUniformLocation(name), v));
}

void Shader::setInt(const std::string& name, int v)
{
	GlCall(glUniform1i(getUniformLocation(name), v));
}

void Shader::setBool(const std::string& name, bool v)
{
	GlCall(glUniform1i(getUniformLocation(name), v ? 1 : 0));
}

void Shader::setVec3(const std::string& name, float v0, float v1, float v2)
{
	GlCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec)
{
	GlCall(glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix)
{
	GlCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
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
			" shader compilation error: " << std::endl;
		std::cout << "Add " << 
			-(int)(vertexShader ? vPreprocessorElements : fPreprocessorElements).size() <<
			" to line numbers.\n" << std::endl;
		std::cout << message << std::endl;

		GlCall(glDeleteShader(0));

		delete[] message;

		//Allow the shader to be recompiled on the fly
		std::cout << "Press ENTER to recompile! ";
		std::cin.get();

		std::string newSource = readFile(
			vertexShader ? vertexFile : fragmentFile,
			vertexShader ? vPreprocessorElements : fPreprocessorElements
		);

		return compileShader(newSource, type);
	}

	return shaderId;
}

std::string Shader::readFile(const std::string& filename, const std::vector<ShaderPreprocessorElement>& preprocessor)
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
				//Include the file from shaders/include/
				includeDir = "res/shader/include/";
			}
			else if (line[includeDirectiveSize] == '\"')
			{
				//Include the file from the current shader's directory
				includeDir = shaderDirectory;
			}
			includedFiles.push_back(includeDir + afterInclude.substr(1, afterInclude.size() - 2));
			const std::string& includeFilepath = includedFiles.back();
			ss << readFile(includeFilepath, preprocessor) << '\n';
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
