#include "Renderer/GlMacros.h"
#include "GBuffer.h"
#include "MathUtils.h"

GBuffer::GBuffer(unsigned int width, unsigned int height, EntityRegistry& registry)
	: gbuf(),
	depthStencilRbo(),
	gbufPosition(width, height, false),
	gbufNormal(width, height, false),
	gbufAlbedoSpec(width, height, false),
	geomPassShader(),
	identityShader(),
	entity(),
	quad()
{
	gbuf.bind();

	gbufPosition.bind();
	gbufPosition.sendToGL(GL_RGB16F, GL_RGB, GL_FLOAT, nullptr);
	gbuf.addAttachment(GL_COLOR_ATTACHMENT0, gbufPosition);

	gbufNormal.bind();
	gbufNormal.sendToGL(GL_RGB16F, GL_RGB, GL_FLOAT, nullptr);
	gbuf.addAttachment(GL_COLOR_ATTACHMENT1, gbufNormal);

	gbufAlbedoSpec.bind();
	gbufAlbedoSpec.sendToGL(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	gbuf.addAttachment(GL_COLOR_ATTACHMENT2, gbufAlbedoSpec);

	gbuf.addMultipleDrawTargets(std::array<unsigned int, 3> { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });

	depthStencilRbo.bind();
	depthStencilRbo.setStorage(GL_DEPTH_COMPONENT, width, height);
	gbuf.addRenderbuffer(depthStencilRbo, GL_DEPTH_ATTACHMENT);
	depthStencilRbo.unbind();
	
	gbuf.checkStatus();
	gbuf.unbind();

	geomPassShader.bind();
	geomPassShader.u_DiffuseSampler = 0;
	geomPassShader.unbind();

	identityShader.bind();
	identityShader.u_ScreenSampler = 0;
	identityShader.unbind();

	registry.getConstructor("anvil").construct(entity);
	entity.moveTo(glm::vec3(0, -2, 0));
}

void GBuffer::renderEntities(const Camera& camera) const
{
	gbuf.bind();
	GlCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	geomPassShader.bind();
	RenderableMaterialModel& material = *entity.getMaterialModel();
	material.getGlModel().vao.bind();
	material.loadAllTextures();
	material.getTexture().promisedFetch().bind(0);
	
	geomPassShader.u_ProjectionMatrix = Math::createProjectionMatrix(0.1f, 1000.0f, 60.0f);
	geomPassShader.u_ViewMatrix = Math::createViewMatrix(camera);
	geomPassShader.u_TransformationMatrix = Math::createTransformationMatrix(entity.position, entity.rotation, 2.0f);

	GlCall(glDrawElements(GL_TRIANGLES, material.getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));

	gbuf.unbind();
	identityShader.bind();
	quad.getVao().bind();
	gbufNormal.bind(0);

	GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GlCall(glEnable(GL_DEPTH_TEST));

	GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));

}
