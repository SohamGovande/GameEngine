#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "../GlMacros.h"
#include "Shader.h"

Shader::Shader(const std::string& filename)
	: rendererID(0), uniformLocationCache()
{
	ShaderProgramSrc src = readSingleFile("res/shaders/" + filename);
	rendererID = createShader(src.vertexSrc, src.fragSrc);
	
}

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
{
	rendererID = createShader(
		readFile("res/shaders/" + vertexFile),
		readFile("res/shaders/" + fragmentFile)
	);
}

Shader::~Shader()
{
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

void Shader::setMatrix4(const std::string& name, const glm::mat4& matrix)
{
	GlCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GlCall(GLuint program = glCreateProgram());
	GLuint vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));

	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

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

		GlCall(glGetShaderInfoLog(shaderId, len, &len, message));
		std::cout << "Shader failed to compile: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
		std::cout << message << std::endl;
		GlCall(glDeleteShader(0));

		delete[] message;
	}

	return shaderId;
}


std::string Shader::readFile(const std::string& filename)
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
		ss << line << '\n';
	
	return ss.str();
}

ShaderProgramSrc Shader::readSingleFile(const std::string& filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	std::stringstream vsh, fsh;
	std::stringstream* current = &vsh;;

	while (getline(stream, line))
	{
		if (line.find("//Shader=") != std::string::npos)
		{
			if (line.find("VERTEX") != std::string::npos)
				current = &vsh;
			else if (line.find("FRAGMENT") != std::string::npos)
				current = &fsh;
			else
				std::cout << "Invalid shader line specified: " << line << std::endl;
		}
		else
		{
			(*current) << line << '\n';
		}
	}

	ShaderProgramSrc src;
	src.fragSrc = fsh.str();
	src.vertexSrc = vsh.str();
	return src;
}

void Shader::bind() const
{
	GlCall(glUseProgram(rendererID));
}

void Shader::unbind() const
{
	GlCall(glUseProgram(0));
}