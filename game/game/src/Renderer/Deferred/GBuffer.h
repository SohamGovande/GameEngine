#pragma once
#include "Renderer/PPE/Framebuffer.h"
#include "Renderer/PPE/Renderbuffer.h"
#include "Renderer/QuadVBO.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader/GBufferGeometryPassShader.h"
#include "Renderer/Shader/IdentityShader.h"
#include "Renderer/Renderers/EntityRenderer.h"
#include "Entity/EntityRegistry.h"

class GBuffer
{
private:
	Framebuffer gbuf;
	Renderbuffer depthStencilRbo;
	Texture gbufPosition;
	Texture gbufNormal;
	Texture gbufAlbedoSpec;

	GBufferGeometryPassShader geomPassShader;
	IdentityShader identityShader;

	Entity entity;
	QuadVBO quad;

public:
	GBuffer(unsigned int width, unsigned int height, EntityRegistry& registry);

	void renderEntities(const Camera& camera) const;

	inline const GBufferGeometryPassShader& getGeomPassShader() const { return geomPassShader; }
	inline GBufferGeometryPassShader& getGeomPassShader() { return geomPassShader; }
};
