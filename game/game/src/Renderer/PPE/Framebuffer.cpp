#include <iostream>
#include "Renderer/GlMacros.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	GlCall(glGenFramebuffers(1, &rendererID));
}

Framebuffer::Framebuffer(Framebuffer&& other)
	: rendererID(other.rendererID)
{
	other.rendererID = 0;
}

Framebuffer::~Framebuffer()
{
	release();
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other)
{
	if (this != &other)
	{
		release();
		rendererID = other.rendererID;
		other.rendererID = 0;
	}
	return *this;
}

void Framebuffer::addAttachment(unsigned int type, const Texture& tex) const
{
	GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, tex.getTextureID(), 0));
}

void Framebuffer::addRenderbuffer(const Renderbuffer& rbo, unsigned int function) const
{
	GlCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, function, GL_RENDERBUFFER, rbo.getRenderbufferID()));
}

void Framebuffer::release() const
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

void Framebuffer::checkStatus() const
{
	unsigned int status = getStatus();
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		const char* msg = GlTranslateError(status);
		std::cout << "[OpenGL error] Incomplete framebuffer: " << msg << std::endl;
		__debugbreak();
	}
}