#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Uniform.h"

void Uniform<float>::update(const float& lastValue) const
{
	glUniform1f(location, lastValue);
}

void Uniform<glm::vec2>::update(const glm::vec2& lastValue) const
{
	glUniform2f(location, lastValue.x, lastValue.y);
}

void Uniform<glm::vec3>::update(const glm::vec3& lastValue) const
{
	glUniform3f(location, lastValue.x, lastValue.y, lastValue.z);
}

void Uniform<glm::vec4>::update(const glm::vec4& lastValue) const
{
	glUniform4f(location, lastValue.x, lastValue.y, lastValue.z, lastValue.w);
}

void Uniform<glm::mat3>::update(const glm::mat3& lastValue) const
{
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(lastValue));
}

void Uniform<glm::mat4>::update(const glm::mat4& lastValue) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lastValue));
}

void Uniform<bool>::update(const bool& lastValue) const
{
	glUniform1i(location, lastValue ? 1 : 0);
}

void Uniform<int>::update(const int& lastValue) const
{
	glUniform1i(location, lastValue);
}