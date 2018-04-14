#include "../GlMacros.h"
#include "TerrainRenderer.h"
#include "MathUtils.h"

TerrainRenderer::TerrainRenderer(Light& light, Shader& shader)
	: light(light), shader(shader)
{

}
void TerrainRenderer::draw(float partialTicks, const Camera& camera, const std::list<Terrain*>& terrains) const
{
	shader.bind();

	shader.setFloat("u_Reflectivity", 0);
	shader.setVec3("u_SkyColor", 1, 1, 1);
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

void TerrainRenderer::renderInstance(float partialTicks, const Terrain& object, const Camera& camera) const
{
	const MaterialModel& model = object.getTerrainModel();

	shader.setMatrix4("u_ViewMatrix", Math::createViewMatrix(camera));

	GlCall(glDrawElements(GL_TRIANGLES, model.getGlModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}