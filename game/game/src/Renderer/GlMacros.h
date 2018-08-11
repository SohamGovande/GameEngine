#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#if defined PROGRAM_DEBUG_MODE

#define ASSERT(x) if (!(x)) __debugbreak();

#define GlCall(x) \
	GlClearError();\
	x;\
	ASSERT(GlLogCall(#x, __FILE__, __LINE__));

#else

#define ASSERT(x) x
#define GlCall(x) x

#endif //PROGRAM_DEBUG_MODE

class GlTranslatedError 
{
private:
	const char* msg;
	bool dynamicallyAllocated;

public:
	GlTranslatedError(const char* msg, bool createCopy);
	GlTranslatedError(const char* msg);
	GlTranslatedError(const GlTranslatedError& other);
	GlTranslatedError(GlTranslatedError&& other);
	~GlTranslatedError();

	GlTranslatedError& operator=(const GlTranslatedError& other);
	GlTranslatedError& operator=(GlTranslatedError&& other);

	operator const char*();
	inline const char* getMessage() const { return msg; }
};

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);
GlTranslatedError GlTranslateError(unsigned int error);