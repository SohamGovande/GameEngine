#include "Renderbuffer.h"
#include "Renderer/GlMacros.h"

Renderbuffer::Renderbuffer()
{
	GlCall(glGenRenderbuffers(1, &rendererID));
}

void Renderbuffer::cleanUp() const
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