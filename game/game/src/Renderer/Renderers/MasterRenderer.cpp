#include "../GlMacros.h"
#include "MasterRenderer.h"
#include "../Shader/Uniform.t.h"

MasterRenderer::MasterRenderer()
	: lights(),
	
	entityRenderer(lights),
	terrainRenderer(*this, lights),

	terrains(),
	wireframe(false),
	needsToUpdateWireframe(true),
	timePassed(0)
{
	lights.emplace_back(glm::vec3(0, 100, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0)); //Sun
}


void MasterRenderer::prepare() const
{
	GlCall(glClearColor(159/255.f, 191/255.f, 244/255.f, 1.0f));

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

void MasterRenderer::addEntity(Entity& entity)
{
	entity.getMaterialModel()->loadAllTextures();

	using BatchIterator = EntityRenderer::Batches::iterator;
	bool shouldInsert = batchedEntities.empty();
	BatchIterator insertionPoint = batchedEntities.begin();

	for (BatchIterator it = batchedEntities.begin(); it != batchedEntities.end(); it++)
	{
		BatchIterator::value_type& batch = *it;
		if (batch.size() == 0)
			continue;
		int comparison = batch.front()->getMaterialModel()->compareAgainstRenderable(*entity.getMaterialModel());
		
		if (comparison == 1)
		{
			insertionPoint = it;
			insertionPoint++;
			shouldInsert = true;
			continue;
		}

		if (comparison == 0)
		{
			batch.push_back(&entity);
			shouldInsert = false;
			break;
		}

		if (comparison == -1)
		{
			insertionPoint = it;
			shouldInsert = true;
			break;
		}
	}

	if (shouldInsert)
		batchedEntities.insert(insertionPoint, BatchIterator::value_type { &entity });
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
	entityRenderer.render(gl, partialTicks, camera, batchedEntities);
}