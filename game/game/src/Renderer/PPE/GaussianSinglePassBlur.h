#pragma once

#include "Renderer/PPE/BlurAxis.h"
#include "Renderer/Shader/GaussianAxisShader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"


template<BlurAxis A>
class GaussianAxisBlur
{
private:
	GaussianAxisShader<A> shader;
	Framebuffer fbo;
	Renderbuffer depthStencilRbo;
	Texture colorBuffer;

public:
	GaussianAxisBlur(unsigned int width, unsigned int height);

	void start() const;
	void stop() const;
	void renderQuad(const VertexArray& quadVao) const;

	inline const Texture& getColorBuffer() const { return colorBuffer; }
	inline Texture& getColorBuffer() { return colorBuffer; }
};

