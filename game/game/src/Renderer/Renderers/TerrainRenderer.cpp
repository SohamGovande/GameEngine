#include "../GlMacros.h"
#include "TerrainRenderer.h"
#include "MathUtils.h"
#include "MasterRenderer.h"

TerrainRenderer::TerrainRenderer(MasterRenderer& masterRenderer, Light& light, Shader& shader)
	: light(light), shader(shader), masterRenderer(masterRenderer)
{
	
}
void TerrainRenderer::draw(float partialTicks, const Camera& camera, const std::list<Terrain*>& terrains)
{
	shader.bind();

	shader.setFloat("u_Reflectivity", 0);
	shader.setVec3("u_SkyColor", 176 / 255.f, 231 / 255.f, 232 / 255.f);
	shader.setVec3("u_LightPos", light.getPosX(), light.getPosY(), light.getPosZ());
	shader.setVec3("u_LightColor", light.getRed(), light.getGreen(), light.getBlue());
	shader.setMatrix4("u_TransformationMatrix", glm::mat4(1.0f));

	for (Terrain* terrain : terrains)
	{
		const MaterialModel& model = terrain->getTerrainModel();
		prepareForRendering(model);
		renderInstance(partialTicks, *terrain, camera);
	}
}


void TerrainRenderer::prepareForRendering(const MaterialModel& material) const
{
	material.getGlModel().vao.bind();
	material.getTexture().promisedFetch().bind(0);
}

void TerrainRenderer::renderInstance(float partialTicks, const Terrain& object, const Camera& camera)
{
	const MaterialModel& model = object.getTerrainModel();

	shader.setMatrix4("u_ViewMatrix", Math::createViewMatrix(camera));

	GlCall(glDrawElements(GL_TRIANGLES, model.getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}