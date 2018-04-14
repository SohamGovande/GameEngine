#include "../GlMacros.h"
#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(float fov, float nearPlane, float farPlane, ResourceMgr& mgr)
	: projectionMatrix(Math::createProjectionMatrix(nearPlane, farPlane, fov)),
	light(0, 100, 0, 1, 1, 1),
	shader("entity/vertex.glsl", "entity/fragment.glsl"),
	
	entityRenderer(light, shader),
	terrainRenderer(light, shader),
	geometryRenderer(light, shader, mgr),

	entities(),
	terrains()
{
	shader.bind();

	pushProjMatIntoShader(shader);
}

MasterRenderer::~MasterRenderer()
{
}

void MasterRenderer::prepare() const
{
	GlCall(glClearColor(176/255.f, 231/255.f, 232/255.f, 255.f));

	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GlCall(glEnable(GL_CULL_FACE));
	GlCall(glCullFace(GL_BACK));

	GlCall(glEnable(GL_DEPTH_TEST));
	GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

//	GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
}


void MasterRenderer::markEntityForRendering(Entity& entity)
{
	MaterialModel& model = *entity.getMaterialModel();
	model.getTexture().load();
	if (model.doesHaveSpecularMap())
		model.getSpecularMap()->load();
	
	auto it = entities.find(model);

	if (it != entities.end())
	{
		it->second.push_back(&entity);
	}
	else
	{
		entities[model] = { &entity };
	}
}

void MasterRenderer::processTerrain(Terrain* terrain)
{
	terrain->getTerrainModel()
		.getTexture()
		.load();
	terrains.push_back(terrain);
}

void MasterRenderer::render(float partialTicks, const Camera& camera)
{
	entityRenderer.draw(partialTicks, camera, entities);
	terrainRenderer.draw(partialTicks, camera, terrains);
//	geometryRenderer.draw(partialTicks, camera);
}