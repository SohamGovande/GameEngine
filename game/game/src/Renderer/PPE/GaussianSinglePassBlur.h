#pragma once
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"

enum BlurType
{
	HORIZONTAL, VERTICAL
};

template<BlurType T>
class GaussianSinglePassBlur
{
private:
	Shader shader;
	Framebuffer fbo;
	Renderbuffer depthStencilRbo;
	Texture colorBuffer;

public:
	GaussianSinglePassBlur(unsigned int width, unsigned int height);
	~GaussianSinglePassBlur();

	void start();
	void stop();
	void renderQuad(const VertexArray& quadVao);
};

