#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "Terrain/Terrain.h"

#include "EntityRenderer.h"
#include "TerrainRenderer.h"

#include "MathUtils.h"
#include "Toolkit.h"

class ResourceMgr;

class MasterRenderer
{
private:
	glm::mat4 projectionMatrix;
	std::vector<Light> lights;
	EntityRenderer entityRenderer;
	TerrainRenderer terrainRenderer;

	Shader shader, tShader;
	
	std::unordered_map<MaterialModel, std::list<Entity*>> entities;
	std::list<Terrain*> terrains;

	bool wireframe, needsToUpdateWireframe;
	
	float timePassed;

public:
	MasterRenderer(float fov, float nearPlane, float farPlane, ResourceMgr& mgr);
	~MasterRenderer();

	void markEntityForRendering(Entity& entity);
	void processTerrain(Terrain& terrain);

	void prepare() const;
	void render(float partialTicks, const Camera& camera);

	void toggleWireframeView() { wireframe = !wireframe; needsToUpdateWireframe = true; }
	
	inline const std::vector<Light>& getLights() const { return lights; }
	inline std::vector<Light>& getLights() { return lights; }
};
