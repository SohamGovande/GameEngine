#include "Framebuffer.h"
#include "Renderer/GlMacros.h"

Framebuffer::Framebuffer()
{
	GlCall(glGenFramebuffers(1, &rendererID));
}

void Framebuffer::addAttachment(unsigned int type, const Texture& tex) const
{
	GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, tex.getTextureID(), 0));
}

void Framebuffer::addRenderbuffer(const Renderbuffer& rbo, unsigned int function) const
{
	GlCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, function, GL_RENDERBUFFER, rbo.getRenderbufferID()));
}

void Framebuffer::cleanUp() const
{
	GlCall(glDeleteFramebuffers(1, &rendererID));
}

void Framebuffer::bind() const
{
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, rendererID));
}

void Framebuffer::unbind() const
{
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int Framebuffer::getStatus() const
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}