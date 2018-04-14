#include "IndexBuffer.h"
#include "GlMacros.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:indicesCount(count)
{
	GlCall(glGenBuffers(1, &rendererID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::cleanUp() const
{
	GlCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
