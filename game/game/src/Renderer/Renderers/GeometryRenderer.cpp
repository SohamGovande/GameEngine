#include <array>
#include <glm/mat3x3.hpp>

#include "../GlMacros.h"
#include "GeometryRenderer.h"
#include "MathUtils.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Physics/CollisionMath.h"
#include "Physics/CollisionCommons.h"

#define AddPoint(x, y, z) {\
Entity ent(*resourceMgr.icosphere);\
ent.scale = 2;\
ent.moveTo(glm::vec3(x, y, z));\
entities.push_back(ent);\
}
#define LineSegmentCount 10
#define AddVec(vec) AddPoint((vec).x, (vec).y, (vec).z)
#define AddLine(start, slope) for (int COUNTER = 0; COUNTER < LineSegmentCount; COUNTER++) AddVec(start + slope * (COUNTER*1.f));
#define AddTri(tri) AddVec(tri[0]);AddVec(tri[1]);AddVec(tri[2]);\
AddLine(tri[0], (tri[1]-tri[0])/(float)LineSegmentCount);\
AddLine(tri[1], (tri[2]-tri[1])/(float)LineSegmentCount);\
AddLine(tri[2], (tri[0]-tri[2])/(float)LineSegmentCount)
#define Transform(mat, tri) for(int i = 0; i < 3; i++)\
{\
tri[i] = mat * tri[i];\
}

using namespace CollisionMath;

GeometryRenderer::GeometryRenderer(Light& light, Shader& shader, ResourceMgr& resourceMgr)
	: light(light),
	shader(shader),
	lastCullingState(true)
{

	std::array<glm::vec3, 3> triangle1{
		glm::vec3(200, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 200),
	};
	std::array<glm::vec3, 3> triangle2{
		glm::vec3(400, 0, 50),
		glm::vec3(150, 0, 100),
		glm::vec3(50, 0, 50),
	};
	CollisionResult result = CollisionDetection::checkTriangleTriangle(
		true,
		triangle1, triangle2
	);

	resourceMgr.icosphere->ensuredFetch()
		.getTexture().load();

	AddTri(triangle1);
	AddTri(triangle2);
	if (result.intersected)
		AddPoint(result.location.x, result.location.y, result.location.z);
}

void GeometryRenderer::draw(float partialTicks, const Camera& camera)
{
	shader.bind();
	shader.setVec3("u_SkyColor", 176 / 255.f, 231 / 255.f, 232 / 255.f);
	shader.setVec3("u_LightPos", light.getPosX(), light.getPosY(), light.getPosZ());
	shader.setVec3("u_LightColor", light.getRed(), light.getGreen(), light.getBlue());

	for (Entity& entity : entities)
	{
		MaterialModel& material = *entity.getMaterialModel();
		
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

		glm::vec3 pos = entity.interpolatePosition(partialTicks);
		glm::vec3 rot = entity.interpolateRotation(partialTicks);

		shader.setMatrix4("u_TransformationMatrix", Math::createTransformationMatrix(pos, rot, entity.scale));
		shader.setMatrix4("u_ViewMatrix", Math::createViewMatrix(camera));

		GlCall(glDrawElements(GL_TRIANGLES, entity.getMaterialModel()->getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
	}
}
