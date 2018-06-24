#include <glm/mat4x4.hpp>

#include <iostream>
#include <array>
#include <functional>
#include <cmath>

#include "EntityRenderer.h"
#include "MathUtils.h"

EntityRenderer::EntityRenderer(const std::vector<Light>& lights, const glm::mat4& projectionMatrix, const std::string& maxLightsStr)
	: lights(lights), 
	shader("entity/vertex.glsl", "entity/fragment.glsl"),
	normalMappedShader("entityNormalMapped/vertex.glsl", "entityNormalMapped/fragment.glsl")
{
	shader.addVertexPreprocessorElement("MAX_LIGHTS", maxLightsStr);
	shader.addFragmentPreprocessorElement("MAX_LIGHTS", maxLightsStr);
	shader.create();
	shader.bind();
	shader.setMat4("u_ProjectionMatrix", projectionMatrix);

	normalMappedShader.addVertexPreprocessorElement("MAX_LIGHTS", maxLightsStr);
	normalMappedShader.addFragmentPreprocessorElement("MAX_LIGHTS", maxLightsStr);
	normalMappedShader.create();
	normalMappedShader.bind();
	normalMappedShader.setMat4("u_ProjectionMatrix", projectionMatrix);
}

EntityRenderer::~EntityRenderer()
{
	shader.cleanUp();
	normalMappedShader.cleanUp();
}

void EntityRenderer::render(GlStateManager& gl, float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities)
{	
	std::array<std::reference_wrapper<Shader>, 2> shaders { shader, normalMappedShader };
	
	for (Shader& shader : shaders)
	{
		shader.bind();
		shader.setVec3("u_SkyColor", 176 / 255.f, 231 / 255.f, 232 / 255.f);

		shader.setInt("u_LightsUsed", lights.size());
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			std::string iString = std::to_string(i);
			shader.setVec3("u_LightPos[" + iString + "]", lights[i].getPos().x, lights[i].getPos().y, lights[i].getPos().z);
			shader.setVec3("u_LightColor[" + iString + "]", lights[i].getColor().r, lights[i].getColor().g, lights[i].getColor().b);
			shader.setFloat("u_LightAttenuation[" + iString + "]", lights[i].getAttenuation());
			shader.setFloat("u_LightBrightness[" + iString + "]", lights[i].getBrightness());
		}
	}

	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		const MaterialModel& material = it->first;
		const std::list<Entity*>& batch = it->second;

		Shader& shader = material.doesHaveNormalMap() ? normalMappedShader : this->shader;
		shader.bind();
		prepareForRendering(gl, shader, material);
		for (const Entity* object : batch)
			renderInstance(partialTicks, shader, *object, camera);
	}
}

void EntityRenderer::prepareForRendering(GlStateManager& gl, Shader& shader, const MaterialModel& material)
{
	if (material.properties.fullyRender)
		gl.disable(GL_CULL_FACE);
	else
		gl.enable(GL_CULL_FACE);

	unsigned int nextTextureID = 0;

	material.getGlModel().vao.bind();
	material.getTexture().promisedFetch().bind(nextTextureID++);

	shader.setFloat("u_Reflectivity", material.properties.reflectivity);
	shader.setFloat("u_ShineDistanceDamper", material.properties.shineDistanceDamper);

	if (material.doesHaveSpecularMap())
	{
		shader.setInt("u_SpecularMap", nextTextureID);
		shader.setBool("u_HasSpecularMap", true);

		material.getSpecularMap().promisedFetch().bind(nextTextureID++);
	}
	else
		shader.setBool("u_HasSpecularMap", false);

	if (material.doesHaveNormalMap())
	{
		shader.setInt("u_NormalMap", nextTextureID);
		material.getNormalMap().promisedFetch().bind(nextTextureID++);
	}
}

void EntityRenderer::renderInstance(float partialTicks, Shader& shader, const Entity& object, const Camera& camera)
{
	glm::vec3 pos = object.interpolatePosition(partialTicks);
	glm::vec3 rot = object.interpolateRotation(partialTicks);

	shader.setMat4("u_TransformationMatrix", Math::createTransformationMatrix(pos, rot, object.scale));
	shader.setMat4("u_ViewMatrix", Math::createViewMatrix(camera));

	GlCall(glDrawElements(GL_TRIANGLES, object.getMaterialModel()->getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}
