#include "../GlMacros.h"
#include "MasterRenderer.h"
#include "../Shader/Uniform.t.h"

MasterRenderer::MasterRenderer(ResourceMgr& mgr)
	: lights(),
	
	entityRenderer(lights),
	terrainRenderer(*this, lights),

	entities(),
	terrains(),
	wireframe(false),
	needsToUpdateWireframe(true),
	timePassed(0)
{
	lights.push_back(Light(glm::vec3(0, 100, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0)));
}

MasterRenderer::~MasterRenderer()
{
}

void MasterRenderer::prepare() const
{
	GlCall(glClearColor(176/255.f, 231/255.f, 232/255.f, 1));

	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GlCall(glEnable(GL_CULL_FACE));
	GlCall(glCullFace(GL_BACK));

	GlCall(glEnable(GL_DEPTH_TEST));
	GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	if (needsToUpdateWireframe)
	{
		GlCall(glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL));
	}
}

void MasterRenderer::markEntityForRendering(Entity& entity)
{
	MaterialModel& model = *entity.getMaterialModel();
	model.getTexture().load();
	if (model.properties.hasSpecularMap())
		model.properties.specularMap->load();
	if (model.properties.hasNormalMap())
		model.properties.normalMap->load();
	if (model.properties.hasParallaxMap())
		model.properties.parallaxMap->load();
	
	auto it = entities.find(model);

	if (it != entities.end())
		it->second.push_back(&entity);
	else
		entities.emplace(model, std::list<Entity*>{ &entity });
}

void MasterRenderer::processTerrain(Terrain& terrain)
{
	for (TerrainTextureInfo& info : terrain.getTextures()) {
		info.texture.load();
		if (info.hasSpecularMap())
			info.specularMap->load();
	}
	terrains.push_back(&terrain);
}

void MasterRenderer::render(float partialTicks, const Camera& camera)
{
	terrainRenderer.render(partialTicks, camera, terrains);
	entityRenderer.render(gl, partialTicks, camera, entities);
}