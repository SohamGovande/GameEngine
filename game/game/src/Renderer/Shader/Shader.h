#pragma once
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

struct ShaderProgramSrc
{
	std::string vertexSrc, fragSrc;
};

class Shader
{
private:
	unsigned int rendererID;

	//caching for uniforms
	std::unordered_map<std::string, int> uniformLocationCache;
public:
	Shader(const std::string& filename);
	Shader(const std::string& vertexFile, const std::string& fragmentFile);
	~Shader();
	
	void bind() const;
	void unbind() const;

	//Uniform accessors
	void setVec4(const std::string& name, float v0, float v1, float v2, float v3);
	void setVec3(const std::string& name, float v0, float v1, float v2);
	void setFloat(const std::string& name, float v);
	void setInt(const std::string& name, int v);
	void setBool(const std::string& name, bool v);
	void setMatrix4(const std::string& name, const glm::mat4& matrix);
	
private:
	int getUniformLocation(const std::string& name);

	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	std::string readFile(const std::string& filename);
	ShaderProgramSrc readSingleFile(const std::string& filePath);
};