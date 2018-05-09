#include "../GlMacros.h"
#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(float fov, float nearPlane, float farPlane, ResourceMgr& mgr)
	: projectionMatrix(Math::createProjectionMatrix(nearPlane, farPlane, fov)),
	light(0, 100, 0, 1, 1, 1),
	shader("entity/vertex.glsl", "entity/fragment.glsl"),
	tShader("terrain/vertex.glsl", "terrain/fragment.glsl"),
	
	entityRenderer(light, shader),
	terrainRenderer(*this, light, tShader),
	geometryRenderer(light, shader, mgr),

	entities(),
	terrains(),
	wireframe(false),
	needsToUpdateWireframe(false)
{
	shader.create();
	shader.bind();
	pushProjMatIntoShader(shader);

	tShader.create();
	tShader.bind();
	pushProjMatIntoShader(tShader);
}

MasterRenderer::~MasterRenderer()
{
	shader.cleanUp();
	tShader.cleanUp();
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

void MasterRenderer::processTerrain(Terrain& terrain)
{
	terrain.getTerrainModel()
		.getTexture()
		.load();
	terrains.push_back(&terrain);
}

void MasterRenderer::render(float partialTicks, const Camera& camera)
{
	terrainRenderer.draw(partialTicks, camera, terrains);
	entityRenderer.draw(partialTicks, camera, entities);
	
//	geometryRenderer.draw(partialTicks, camera);
}