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

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);
