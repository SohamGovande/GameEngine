#pragma once
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "../Camera.h"
#include "../Shader/Shader.h"
#include "../MaterialModel.h"
#include "../Light.h"

class ResourceMgr;

class GeometryRenderer
{
private:
	Light& light;
	Shader& shader;
	glm::mat4 projectionMatrix;
	bool lastCullingState;

	std::vector<Entity> entities;

public:
	GeometryRenderer(Light& light, Shader& shader, ResourceMgr& mgr);

	void draw(float partialTicks, const Camera& camera);

	inline void loadProjectionMatrix(glm::mat4 mat) { projectionMatrix = mat; }
};

