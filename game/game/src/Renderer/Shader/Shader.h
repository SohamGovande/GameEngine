#pragma once
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include "ShaderPreprocessorElement.h"

struct ShaderProgramSrc
{
	std::string vertexSrc, fragSrc;
};

class Shader
{
private:
	unsigned int rendererID, vs, fs;

	//caching for uniforms
	std::unordered_map<std::string, int> uniformLocationCache;
	std::vector<ShaderPreprocessorElement> vPreprocessorElements;
	std::vector<ShaderPreprocessorElement> fPreprocessorElements;

	std::string vertexFile, fragmentFile, shaderDirectory;
	std::vector<std::string> includedFiles;

public:
	Shader(const std::string& vertexFile, const std::string& fragmentFile,
		std::vector<ShaderPreprocessorElement>&& vPreprocessors = std::vector<ShaderPreprocessorElement>(),
		std::vector<ShaderPreprocessorElement>&& fPreprocessors = std::vector<ShaderPreprocessorElement>());
	Shader(const Shader& other) = delete;
	Shader(Shader&& other);
	~Shader();

	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other);

	void release();

	void bind() const;
	void unbind() const;

	//Uniform accessors
	void setVec4(const std::string& name, float v0, float v1, float v2, float v3);
	void setVec4(const std::string& name, const glm::vec4& vec);
	void setVec3(const std::string& name, float v0, float v1, float v2);
	void setVec3(const std::string& name, const glm::vec3& vec);
	void setFloat(const std::string& name, float v);
	void setInt(const std::string& name, int v);
	void setBool(const std::string& name, bool v);
	void setMat4(const std::string& name, const glm::mat4& matrix);
	
private:
	int getUniformLocation(const std::string& name);

	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	std::string readFile(const std::string& filename, const std::vector<ShaderPreprocessorElement>& preprocessor);
};