#pragma once
#include <vector>
#include "ShaderVariable.h"
#include "Struct.h"

class ShaderFile
{
private:
	std::vector<ShaderVariable> uniforms;
	std::vector<Struct> structs;

private:
	void recursivelyRemove(const std::vector<ShaderVariable>& vars, std::vector<std::vector<Struct>::const_iterator>& structsToErase);

public:

	void removeNonUniformStructs();

	const Struct* getStruct(const std::string& name) const;
	Struct* getStruct(const std::string& name);
	
	std::vector<Struct>::const_iterator getStructIt(const std::string& name) const;
	std::vector<Struct>::iterator getStructIt(const std::string& name);

	inline const std::vector<ShaderVariable>& getUniforms() const { return uniforms; }
	inline std::vector<ShaderVariable>& getUniforms() { return uniforms; }
	inline const std::vector<Struct>& getStructs() const { return structs; }
	inline std::vector<Struct>& getStructs() { return structs; }
};

