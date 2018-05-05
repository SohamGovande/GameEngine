#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "Terrain/Terrain.h"

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "GeometryRenderer.h"

#include "MathUtils.h"
#include "GlfwUtils.h"

class ResourceMgr;

class MasterRenderer
{
private:
	glm::mat4 projectionMatrix;
	Light light;
	EntityRenderer entityRenderer;
	TerrainRenderer terrainRenderer;
	GeometryRenderer geometryRenderer;

	Shader shader, tShader;
	
	std::unordered_map<MaterialModel, std::list<Entity*>> entities;
	std::list<Terrain*> terrains;

	bool wireframe, needsToUpdateWireframe;
private:
	
public:
	MasterRenderer(float fov, float nearPlane, float farPlane, ResourceMgr& mgr);
	~MasterRenderer();

	void markEntityForRendering(Entity& entity);
	void processTerrain(Terrain& terrain);

	void prepare() const;
	void render(float partialTicks, const Camera& camera);

	void toggleWireframeView() { wireframe = !wireframe; needsToUpdateWireframe = true; }
	
	inline void pushProjMatIntoShader(Shader& shader) const { shader.setMatrix4("u_ProjectionMatrix", projectionMatrix); }
};
