#include "IndexBuffer.h"
#include "GlMacros.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:indicesCount(count)
{
	GlCall(glGenBuffers(1, &rendererID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other)
	: rendererID(other.rendererID), indicesCount(other.indicesCount)
{
	other.rendererID = 0;
	other.indicesCount = 0;
}

IndexBuffer::~IndexBuffer()
{
	release();
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other)
{
	if (this != &other)
	{
		release();

		rendererID = other.rendererID;
		indicesCount = other.indicesCount;
		other.rendererID = 0;
		other.indicesCount = 0;
	}
	return *this;
}

void IndexBuffer::bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::release()
{
	GlCall(glDeleteBuffers(1, &rendererID));
}
