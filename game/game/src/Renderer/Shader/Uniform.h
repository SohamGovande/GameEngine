#pragma once
#include "Shader.h"
#include "../GlMacros.h"

template<typename T>
class Uniform
{
private:
	unsigned int location;
	T lastValue;
	bool changed;

public:

	inline Uniform()
		: changed(false)
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

	inline void set(const T& value) 
	{
		changed = (lastValue != value);
		if (changed)
			lastValue = value;
	}

	inline void setAndUpdate(const T& value)
	{
		set(value);
		update();
	}

	//Use template specializations
	void update()
	{
		static_assert(false, "No template specialization for this Uniform type! Please create one in Uniform.cpp for Uniform#update().");
	}
};