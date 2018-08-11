#pragma once
#include "Renderer/Texture.h"
#include "Renderbuffer.h"

class Framebuffer
{
private:
	unsigned int rendererID;

public:
	Framebuffer();
	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer(Framebuffer&& other);
	~Framebuffer();

	Framebuffer& operator=(const Framebuffer& other) = delete;
	Framebuffer& operator=(Framebuffer&& other);

	void addAttachment(unsigned int type, const Texture& tex) const;
	void addRenderbuffer(const Renderbuffer& rbo, unsigned int function) const;
	
	template<unsigned int N>
	inline void addMultipleDrawTargets(const std::array<unsigned int, N>& targets) const
	{
		GlCall(glDrawBuffers(N, &targets.front()));
	}

	unsigned int getStatus() const;

	void release() const;
	void bind() const;
	void unbind() const;
};
