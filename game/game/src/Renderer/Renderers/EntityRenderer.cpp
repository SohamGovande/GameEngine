#include <glm/mat4x4.hpp>

#include <iostream>
#include <array>
#include <functional>
#include <cmath>

#include "EntityRenderer.t.h"
#include "Renderer/Shader/SyncWithUniform.t.h"
#include "MathUtils.h"

EntityRenderer::EntityRenderer(const std::vector<Light>& lights)
	: lights(lights)
{
}

void EntityRenderer::render(GlStateManager& gl, float partialTicks, const Camera& camera, const std::vector<std::vector<Entity*>>& batchedEntities)
{	
	prepareShader(nShader);
	prepareShader(nmShader);
	prepareShader(pmShader);

	for (const BatchType& batch : batchedEntities)
	{
		if (batch.size() == 0)
			continue;

		const RenderableMaterialModel& materialModel = *batch.front()->getMaterialModel();
		const MaterialModelProperties& properties = materialModel.properties;

		if (properties.hasParallaxMap())
			renderBatch(partialTicks, gl, pmShader, materialModel, batch, camera);
		else if (properties.hasNormalMap())
			renderBatch(partialTicks, gl, nmShader, materialModel, batch, camera);
		else
			renderBatch(partialTicks, gl, nShader, materialModel, batch, camera);
	}
}

