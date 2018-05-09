#pragma once
#include "Renderer/Texture.h"
#include "Renderbuffer.h"

class Framebuffer
{
private:
	unsigned int rendererID;

public:
	Framebuffer();

	void addAttachment(unsigned int type, const Texture& tex) const;
	void addRenderbuffer(const Renderbuffer& rbo, unsigned int function) const;

	unsigned int getStatus() const;

	void cleanUp() const;
	void bind() const;
	void unbind() const;
};
