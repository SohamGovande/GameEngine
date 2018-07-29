#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Uniform.h"

void Uniform<float>::update()
{
	if (changed) { glUniform1f(location, lastValue); changed = false; }
}

void Uniform<glm::vec2>::update()
{
	if (changed) { glUniform2f(location, lastValue.x, lastValue.y); changed = false; }
}

void Uniform<glm::vec3>::update()
{
	if (changed) { glUniform3f(location, lastValue.x, lastValue.y, lastValue.z); changed = false; }
}

void Uniform<glm::vec4>::update()
{
	if (changed) { glUniform4f(location, lastValue.x, lastValue.y, lastValue.z, lastValue.w); changed = false; }
}

void Uniform<glm::mat3>::update()
{
	if (changed) { glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(lastValue)); changed = false; }
}

void Uniform<glm::mat4>::update()
{
	if (changed) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lastValue)); changed = false; }
}

void Uniform<bool>::update()
{
	if (changed) { glUniform1i(location, lastValue ? 1 : 0); changed = false; }
}

void Uniform<int>::update()
{
	if (changed) { glUniform1i(location, lastValue); changed = false; }
}