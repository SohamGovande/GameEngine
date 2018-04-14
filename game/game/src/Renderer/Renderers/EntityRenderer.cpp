#include <glm/mat4x4.hpp>

#include <iostream>
#include <cmath>

#include "../GlMacros.h"
#include "EntityRenderer.h"
#include "MathUtils.h"

EntityRenderer::EntityRenderer(Light& light, Shader& shader)
	: light(light),
	shader(shader),
	lastCullingState(true)
{
	
}

void EntityRenderer::draw(float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities)
{
	shader.bind();
	shader.setVec3("u_SkyColor", 176/255.f, 231/255.f, 232/255.f);
	shader.setVec3("u_LightPos", light.getPosX(), light.getPosY(), light.getPosZ());
	shader.setVec3("u_LightColor", light.getRed(), light.getGreen(), light.getBlue());

	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		const MaterialModel& material = it->first;
		const std::list<Entity*>& batch = it->second;
		prepareForRendering(material);

		for (const Entity* object : batch)
			renderInstance(partialTicks, *object, camera);
	}
}

void EntityRenderer::prepareForRendering(const MaterialModel& material)
{
	if (lastCullingState != !material.fullyRender)
	{
		if (material.fullyRender)
		{
			GlCall(glDisable(GL_CULL_FACE));
		}
		else
		{
			GlCall(glEnable(GL_CULL_FACE));
		}

		lastCullingState = !material.fullyRender;
	}

	material.getGlModel().vao.bind();
	material.getTexture().promisedFetch().bind(0);

	shader.setFloat("u_Reflectivity", material.reflectivity);
	shader.setFloat("u_ShineDistanceDamper", material.shineDistanceDamper);

	if (material.doesHaveSpecularMap())
	{
		material.getSpecularMap()->promisedFetch().bind(1);

		shader.setInt("u_SpecularMap", 1);
		shader.setBool("u_HasSpecularMap", true);
	}
	else
		shader.setBool("u_HasSpecularMap", false);
}

void EntityRenderer::renderInstance(float partialTicks, const Entity& object, const Camera& camera) const
{
	glm::vec3 pos = object.interpolatePosition(partialTicks);
	glm::vec3 rot = object.interpolateRotation(partialTicks);

	shader.setMatrix4("u_TransformationMatrix", Math::createTransformationMatrix(pos, rot, object.scale));
	shader.setMatrix4("u_ViewMatrix", Math::createViewMatrix(camera));

	GlCall(glDrawElements(GL_TRIANGLES, object.getMaterialModel()->getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}
