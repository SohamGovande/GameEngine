#include <sstream>
#include <iostream>

#include "GlMacros.h"

void GlClearError()
{
	//Clear all the errors
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	bool errorExists = false;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::stringstream stream;
		stream << std::hex << error;

		errorExists = true;

		std::cout << "[OpenGL error]: 0x" << stream.str() << " while executing " << function << " from line " << line << " in " << file << std::endl;
	}
	return !errorExists;
}