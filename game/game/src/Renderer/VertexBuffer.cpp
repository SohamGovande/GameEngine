#include "GlMacros.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GlCall(glGenBuffers(1, &rendererID));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
	: rendererID(other.rendererID)
{
	other.rendererID = 0;
}

VertexBuffer::~VertexBuffer()
{
	release();
}

void VertexBuffer::release()
{
	GlCall(glDeleteBuffers(1, &rendererID));
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
	if (this != &other)
	{
		release();
		rendererID = other.rendererID;
		other.rendererID = 0;
	}
	return *this;
}

void VertexBuffer::bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
