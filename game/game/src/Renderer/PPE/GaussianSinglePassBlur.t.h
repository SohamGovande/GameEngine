#pragma once
#include "GaussianSinglePassBlur.h"
#include "Renderer/GlMacros.h"

template<BlurAxis A>
GaussianAxisBlur<A>::GaussianAxisBlur(unsigned int width, unsigned int height)
	: shader(),
	fbo(), depthStencilRbo(),
	colorBuffer(width, height, true)
{
	shader.bind();
	shader.u_ScreenSampler.uncheckedSet(0);
	shader.u_PixelDensity.uncheckedSet(1.0f / (A == BlurAxis::HORIZONTAL ? width : height));

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

template<BlurAxis T>
GaussianAxisBlur<T>::~GaussianAxisBlur()
{
	fbo.cleanUp();
	depthStencilRbo.cleanUp();
	colorBuffer.cleanUp();
}

template<BlurAxis T>
void GaussianAxisBlur<T>::start()
{
	fbo.bind();
	GlCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GlCall(glEnable(GL_DEPTH_TEST));
}

template<BlurAxis T>
void GaussianAxisBlur<T>::stop()
{
	fbo.unbind();
}

template<BlurAxis T>
void GaussianAxisBlur<T>::renderQuad(const VertexArray& quadVao)
{
	GlCall(glClearColor(1.f, 0.f, 1.f, 1.f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));

	shader.bind();
	quadVao.bind();
	GlCall(glDisable(GL_DEPTH_TEST));
	colorBuffer.bind(0);
	GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}
