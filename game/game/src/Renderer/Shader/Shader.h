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

	const std::string vertexFile, fragmentFile;
public:
	Shader(const std::string& vertexFile, const std::string& fragmentFile);
	
	void create();

	inline void addVertexPreprocessorElement(const std::string& name, const std::string& value)
	{
		vPreprocessorElements.emplace_back(name, value);
	}
	inline void addFragmentPreprocessorElement(const std::string& name, const std::string& value)
	{
		fPreprocessorElements.emplace_back(name, value);
	}

	void bind() const;
	void unbind() const;

	//Uniform accessors
	void setVec4(const std::string& name, float v0, float v1, float v2, float v3);
	void setVec3(const std::string& name, float v0, float v1, float v2);
	void setFloat(const std::string& name, float v);
	void setInt(const std::string& name, int v);
	void setBool(const std::string& name, bool v);
	void setMat4(const std::string& name, const glm::mat4& matrix);
	
	void cleanUp() const;
private:
	int getUniformLocation(const std::string& name);

	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	std::string readFile(const std::string& filename, const std::vector<ShaderPreprocessorElement>& preprocessor);
};