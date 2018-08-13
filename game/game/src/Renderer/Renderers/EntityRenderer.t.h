#pragma once
#include "EntityRenderer.h"

template<typename ShaderType>
void EntityRenderer::prepareShader(ShaderType& shader)
{
	shader.bind();
	shader.u_SkyColor = glm::vec3(159/255.f, 191/255.f, 244/255.f);

	shader.u_LightsUsed = lights.size();

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		ShaderType::FsPointLight& light = shader.u_PointLights[i];

		shader.u_LightPos[i] = lights[i].getPos();
		light.color = lights[i].getColor();
		light.attenuation = lights[i].getAttenuation();
	}
}

template<typename ShaderType>
inline void EntityRenderer::renderBatch(
	float partialTicks,
	GlStateManager& gl,
	ShaderType& shader,
	const RenderableMaterialModel& material, 
	const std::vector<Entity*>& batch,
	const Camera& camera
)
{
	shader.bind();
	prepareShaderForRendering(gl, shader, material);

	const MaterialModelProperties& properties = material.properties;

	for (const Entity* entity : batch)
	{
		renderInstance(
			partialTicks,
			shader,
			*entity,
			camera
		);
	}
}

template<typename ShaderType>
inline void EntityRenderer::renderInstance(float partialTicks, ShaderType& shader, const Entity & object, const Camera & camera)
{
	glm::vec3 pos = object.interpolatePosition(partialTicks);
	glm::vec3 rot = object.interpolateRotation(partialTicks);

	shader.u_TransformationMatrix = Math::createTransformationMatrix(pos, rot, object.scale);
	shader.u_ViewMatrix = Math::createViewMatrix(camera);

	GlCall(glDrawElements(GL_TRIANGLES, object.getMaterialModel()->getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}

template<typename ShaderType>
inline void EntityRenderer::prepareGenericShaderForRendering(GlStateManager& gl, ShaderType& shader, const RenderableMaterialModel& material, unsigned int& nextTextureID)
{
	if (material.properties.fullyRender)
		gl.disable(GL_CULL_FACE);
	else
		gl.enable(GL_CULL_FACE);

	material.getGlModel().vao.bind();
	material.getTexture().promisedFetch().bind(nextTextureID++);

	shader.u_Reflectivity = material.properties.reflectivity;
	shader.u_ShineDistanceDamper = material.properties.shineDistanceDamper;

	if (material.properties.hasSpecularMap())
	{
		shader.u_HasSpecularMap = true;
		shader.u_SpecularMap = nextTextureID;

		material.properties.specularMap->promisedFetch().bind(nextTextureID++);
	}
	else
		shader.u_HasSpecularMap = false;
}

template<typename ShaderType>
inline void EntityRenderer::prepareShaderForRendering(GlStateManager& gl, ShaderType& shader, const RenderableMaterialModel& material)
{
	unsigned int nextTextureID = 0;
	prepareGenericShaderForRendering(gl, shader, material, nextTextureID);
}

template<>
inline void EntityRenderer::prepareShaderForRendering<NormalMappedEntityShader>(
	GlStateManager& gl,
	NormalMappedEntityShader& shader, 
	const RenderableMaterialModel& material
)
{
	unsigned int nextTextureID = 0;
	prepareGenericShaderForRendering(gl, shader, material, nextTextureID);

	//Normal map
	shader.u_NormalMap = nextTextureID;
	material.properties.normalMap->promisedFetch().bind(nextTextureID++);
}

template<>
inline void EntityRenderer::prepareShaderForRendering<ParallaxMappedEntityShader>(
	GlStateManager& gl,
	ParallaxMappedEntityShader& shader, 
	const RenderableMaterialModel& material
)
{
	unsigned int nextTextureID = 0;
	prepareGenericShaderForRendering(gl, shader, material, nextTextureID);

	//Normal map
	shader.u_NormalMap = nextTextureID;
	material.properties.normalMap->promisedFetch().bind(nextTextureID++);

	//Parallax map
	shader.u_ParallaxMap = nextTextureID;
	shader.u_ParallaxMapAmplitude = material.properties.parallaxMapAmplitude;

	material.properties.parallaxMap->promisedFetch().bind(nextTextureID++);
}