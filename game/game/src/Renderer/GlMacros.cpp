#include <sstream>
#include <iostream>

#include "GlMacros.h"

GlTranslatedError::GlTranslatedError(const char* msg, bool createCopy)
	: dynamicallyAllocated(createCopy)
{
	this->msg = createCopy ? _strdup(msg) : msg;
}

GlTranslatedError::GlTranslatedError(const char* msg)
	: GlTranslatedError(msg, false)
{
}

GlTranslatedError::GlTranslatedError(const GlTranslatedError& other)
	: dynamicallyAllocated(other.dynamicallyAllocated)
{
	this->msg = dynamicallyAllocated ? _strdup(other.msg) : other.msg;
}

GlTranslatedError::GlTranslatedError(GlTranslatedError&& other)
	: dynamicallyAllocated(other.dynamicallyAllocated), msg(other.msg)
{
	other.dynamicallyAllocated = false;
}

GlTranslatedError::~GlTranslatedError()
{
	if (dynamicallyAllocated)
		delete[] msg;
}

GlTranslatedError& GlTranslatedError::operator=(const GlTranslatedError& other)
{
	if (this != &other)
	{
		this->dynamicallyAllocated = other.dynamicallyAllocated;
		this->msg = dynamicallyAllocated ? _strdup(other.msg) : other.msg;
	}
	return *this;
}

GlTranslatedError& GlTranslatedError::operator=(GlTranslatedError&& other)
{
	if (this != &other)
	{
		dynamicallyAllocated = other.dynamicallyAllocated;
		msg = other.msg;
		other.dynamicallyAllocated = false;
	}
	return *this;
}

GlTranslatedError::operator const char*()
{
	return msg;
}

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
		errorExists = true;

		std::cout << "[OpenGL error]: " << GlTranslateError(error) << " while executing " << function << " from line " << line << " in " << file << std::endl;
	}
	return !errorExists;
}

GlTranslatedError GlTranslateError(unsigned int error)
{
	switch (error)
	{
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
	case GL_FRAMEBUFFER_UNSUPPORTED:
		return "GL_FRAMEBUFFER_UNSUPPORTED";
	}

	std::stringstream errstream;
	errstream << "0x" << std::hex << error << " (no translation key)";
	return GlTranslatedError(errstream.str().c_str(), true);
}
