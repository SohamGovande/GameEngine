#include "Renderbuffer.h"
#include "Renderer/GlMacros.h"

Renderbuffer::Renderbuffer()
{
	GlCall(glGenRenderbuffers(1, &rendererID));
}

Renderbuffer::Renderbuffer(Renderbuffer&& other)
	: rendererID(other.rendererID)
{
	other.rendererID = 0;
}

Renderbuffer::~Renderbuffer()
{
	release();
}

Renderbuffer& Renderbuffer::operator=(Renderbuffer&& other)
{
	if (this != &other)
	{
		release();
		rendererID = other.rendererID;
		other.rendererID = 0;
	}
	return *this;
}

void Renderbuffer::release() const
{
	GlCall(glDeleteRenderbuffers(1, &rendererID));
}

void Renderbuffer::bind() const
{
	GlCall(glBindRenderbuffer(GL_RENDERBUFFER, rendererID));
}

void Renderbuffer::unbind() const
{
	GlCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void Renderbuffer::setStorage(unsigned int storage, unsigned int width, unsigned int height) const
{
	GlCall(glRenderbufferStorage(GL_RENDERBUFFER, storage, width, height));
}