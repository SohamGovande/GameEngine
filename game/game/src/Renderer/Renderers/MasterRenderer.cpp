#include "../GlMacros.h"
#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(float fov, float nearPlane, float farPlane, ResourceMgr& mgr)
	: projectionMatrix(Math::createProjectionMatrix(nearPlane, farPlane, fov)),
	lights(),
	
	entityRenderer(lights, projectionMatrix, MAX_LIGHTS_STR),
	terrainRenderer(*this, lights, projectionMatrix, MAX_LIGHTS_STR),

	entities(),
	terrains(),
	wireframe(false),
	needsToUpdateWireframe(false),
	timePassed(0)
{
	lights.emplace_back(glm::vec3(0, 100, 0), glm::vec3(1, 1, 1), 0.0f, 1.0f);
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
	if (model.doesHaveSpecularMap())
		model.getSpecularMap().load();
	if (model.doesHaveNormalMap())
		model.getNormalMap().load();
	
	std::unordered_map<MaterialModel, std::list<Entity*>>::iterator it = entities.find(model);

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