#include "TestEffect.h"
#include "Renderer/GlMacros.h"

TestEffect::TestEffect(unsigned int width, unsigned int height)
	: shader("fbo/vertex.glsl","fbo/fragment.glsl"),
	fbo(), depthStencilRbo(),
	colorBuffer(width, height)
{
	shader.create();
	shader.bind();
	shader.setInt("u_ScreenSampler", 0);

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

TestEffect::~TestEffect()
{
	shader.unbind();
	shader.cleanUp();
	fbo.cleanUp();
	depthStencilRbo.cleanUp();
	colorBuffer.cleanUp();
}

void TestEffect::start()
{
	fbo.bind();
	GlCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GlCall(glEnable(GL_DEPTH_TEST));
}

void TestEffect::stop()
{
	fbo.unbind();
}

void TestEffect::renderQuad(const VertexArray & quadVao)
{
	GlCall(glClearColor(1.f, 0.f, 1.f, 1.f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));

	shader.bind();
	quadVao.bind();
	GlCall(glDisable(GL_DEPTH_TEST));
	colorBuffer.bind();
	GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}
