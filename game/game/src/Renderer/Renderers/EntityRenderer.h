#pragma once
#include <glm/mat4x4.hpp>

#include "Renderer/Camera.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/MaterialModel.h"
#include "Renderer/Light.h"
#include "Renderer/GlStateManager.h"

#include "Renderer/Shader/NormalEntityShader.h"
#include "Renderer/Shader/NormalMappedEntityShader.h"
#include "Renderer/Shader/ParallaxMappedEntityShader.h"

class EntityRenderer
{
private:
	const std::vector<Light>& lights;

	NormalEntityShader nShader;
	NormalMappedEntityShader nmShader;
	ParallaxMappedEntityShader pmShader;

private:
	template<typename ShaderType>
	void prepareGenericShaderForRendering(GlStateManager& gl, ShaderType& shader, const RenderableMaterialModel& material, unsigned int& nextTextureID);
	
	template<typename ShaderType>
	void prepareShaderForRendering(GlStateManager& gl, ShaderType& shader, const RenderableMaterialModel& material);

	template<typename ShaderType>
	void renderInstance(float partialTicks, ShaderType& shader, const Entity& object, const Camera& camera);
	
	template<typename ShaderType>
	void prepareShader(ShaderType& shader);

	template<typename ShaderType>
	void renderBatch(
		float partialTicks,
		GlStateManager& gl,
		ShaderType& shader,
		const RenderableMaterialModel& model,
		const std::list<Entity*>& batch,
		const Camera& camera
	);
public:
	EntityRenderer(const std::vector<Light>& lights);

	void render(GlStateManager& gl, float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities);

	inline const NormalMappedEntityShader& getNormalMappedShader() const { return nmShader; }
	inline NormalMappedEntityShader& getNormalMappedShader() { return nmShader; }

	inline const ParallaxMappedEntityShader& getParallaxMappedShader() const { return pmShader; }
	inline ParallaxMappedEntityShader& getParallaxMappedShader() { return pmShader; }

	inline const NormalEntityShader& getRegularShader() const { return nShader; }
	inline NormalEntityShader& getRegularShader() { return nShader; }
};
