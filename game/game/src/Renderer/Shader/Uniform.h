#pragma once
#include "Shader.h"
#include "../GlMacros.h"

template<typename T>
class Uniform
{
private:
	unsigned int location;

public:

	inline Uniform()
	{
	}

	inline void loadLocation(const std::string& name, unsigned int programID)
	{
		location = glGetUniformLocation(programID, &name.front());
	}

	inline void loadLocation(const char* name, unsigned int programID)
	{
		location = glGetUniformLocation(programID, name);
	}

	inline void uncheckedSet(const T& value) const
	{
		update(value);
	}

	inline void operator=(const T& value) const
	{
		uncheckedSet(value);
	}

	//Use template specializations
	void update(const T& value) const
	{
		static_assert(false, "No template specialization for this Uniform type! Please create one in Uniform.cpp for Uniform#update().");
	}
};