#include "../GlMacros.h"
#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(float fov, float nearPlane, float farPlane, ResourceMgr& mgr)
	: projectionMatrix(Math::createProjectionMatrix(nearPlane, farPlane, fov)),
	lights(),
	shader("entity/vertex.glsl", "entity/fragment.glsl"),
	tShader("terrain/vertex.glsl", "terrain/fragment.glsl"),
	
	entityRenderer(lights, shader),
	terrainRenderer(*this, lights, tShader),

	entities(),
	terrains(),
	wireframe(false),
	needsToUpdateWireframe(false),
	timePassed(0)
{
	lights.emplace_back(glm::vec3(0, 100, 0), glm::vec3(1, 1, 1), 0, .25f);
	lights.emplace_back(glm::vec3(0, 50, 0), glm::vec3(.92f, .78f, .18f), 0.05f, 1.f);
		
	const std::string MAX_LIGHTS_STR = std::to_string(MAX_LIGHTS);

	shader.addVertexPreprocessorElement("MAX_LIGHTS", MAX_LIGHTS_STR);
	shader.addFragmentPreprocessorElement("MAX_LIGHTS", MAX_LIGHTS_STR);
	shader.create();
	shader.bind();
	shader.setMatrix4("u_ProjectionMatrix", projectionMatrix);

	tShader.addVertexPreprocessorElement("MAX_LIGHTS", MAX_LIGHTS_STR);
	tShader.addFragmentPreprocessorElement("MAX_LIGHTS", MAX_LIGHTS_STR);
	tShader.create();
	tShader.bind();
	tShader.setMatrix4("u_ProjectionMatrix", projectionMatrix);
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
}