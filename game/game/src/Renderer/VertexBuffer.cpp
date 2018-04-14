#include "GlMacros.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GlCall(glGenBuffers(1, &rendererID));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::cleanUp() const
{
	GlCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
