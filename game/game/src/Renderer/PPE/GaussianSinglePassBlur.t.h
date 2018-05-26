#pragma once
#include "GaussianSinglePassBlur.h"
#include "Renderer/GlMacros.h"

template<BlurType T>
GaussianSinglePassBlur<T>::GaussianSinglePassBlur(unsigned int width, unsigned int height)
	: shader("gaussianBlur/vertex.glsl","gaussianBlur/fragment.glsl"),
	fbo(), depthStencilRbo(),
	colorBuffer(width, height, true)
{
	shader.addVertexPreprocessorElement("BLUR_DIRECTION", T == HORIZONTAL ? "HORIZONTAL" : "VERTICAL");
	shader.create();
	shader.bind();
	shader.setInt("u_ScreenSampler", 0);
	shader.setFloat("u_PixelDensity", 1.0f / (T == HORIZONTAL ? width : height));

	fbo.bind();
	colorBuffer.unbind();
	fbo.addAttachment(GL_COLOR_ATTACHMENT0, colorBuffer);

	depthStencilRbo.bind();
	depthStencilRbo.setStorage(GL_DEPTH24_STENCIL8, width, height);
	depthStencilRbo.unbind();

	fbo.addRenderbuffer(depthStencilRbo, GL_DEPTH_STENCIL_ATTACHMENT);

	ASSERT(fbo.getStatus() == GL_FRAMEBUFFER_COMPLETE);

	fbo.unbind();
}

template<BlurType T>
GaussianSinglePassBlur<T>::~GaussianSinglePassBlur()
{
	shader.unbind();
	shader.cleanUp();
	fbo.cleanUp();
	depthStencilRbo.cleanUp();
	colorBuffer.cleanUp();
}

template<BlurType T>
void GaussianSinglePassBlur<T>::start()
{
	fbo.bind();
	GlCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GlCall(glEnable(GL_DEPTH_TEST));
}

template<BlurType T>
void GaussianSinglePassBlur<T>::stop()
{
	fbo.unbind();
}

template<BlurType T>
void GaussianSinglePassBlur<T>::renderQuad(const VertexArray& quadVao)
{
	GlCall(glClearColor(1.f, 0.f, 1.f, 1.f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));

	shader.bind();
	quadVao.bind();
	GlCall(glDisable(GL_DEPTH_TEST));
	colorBuffer.bind();
	GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}
