#include <glm/mat4x4.hpp>

#include <iostream>
#include <array>
#include <functional>
#include <cmath>

#include "EntityRenderer.t.h"
#include "MathUtils.h"

EntityRenderer::EntityRenderer(const std::vector<Light>& lights, const glm::mat4& projectionMatrix, const std::string& maxLightsStr)
	: lights(lights)
{
	nShader.bind();
	nShader.u_ProjectionMatrix.setAndUpdate(projectionMatrix);

	nmShader.bind();
	nmShader.u_ProjectionMatrix.setAndUpdate(projectionMatrix);

	pmShader.bind();
	pmShader.u_ProjectionMatrix.setAndUpdate(projectionMatrix);
}

EntityRenderer::~EntityRenderer()
{
}

void EntityRenderer::render(GlStateManager& gl, float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities)
{	
	prepareShader(nShader);
	prepareShader(nmShader);
	prepareShader(pmShader);

	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		const MaterialModel& material = it->first;
		const MaterialModelProperties& properties = material.properties;
		const std::list<Entity*>& batch = it->second;

		if (batch.size() > 0) 
		{
			const RenderableMaterialModel& renderableModel = *batch.front()->getMaterialModel();

			if (properties.hasParallaxMap())
				renderBatch(partialTicks, gl, pmShader, renderableModel, batch, camera);
			else if (properties.hasNormalMap())
				renderBatch(partialTicks, gl, nmShader, renderableModel, batch, camera);
			else
				renderBatch(partialTicks, gl, nShader, renderableModel, batch, camera);
		}
	}
}

