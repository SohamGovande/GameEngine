#pragma once
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"

class TestEffect
{
private:
	Shader shader;
	Framebuffer fbo;
	Renderbuffer depthStencilRbo;
	Texture colorBuffer;

public:
	TestEffect(unsigned int width, unsigned int height);
	~TestEffect();

	void start();
	void stop();
	void renderQuad(const VertexArray& quadVao);
};
