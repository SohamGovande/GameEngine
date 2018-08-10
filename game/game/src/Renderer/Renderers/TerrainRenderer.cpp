#include "../GlMacros.h"
#include "TerrainRenderer.h"
#include "MathUtils.h"
#include "MasterRenderer.h"

TerrainRenderer::TerrainRenderer(MasterRenderer& masterRenderer, const std::vector<Light>& lights)
	: lights(lights),
	terrainShader(),
	masterRenderer(masterRenderer)
{
}

void TerrainRenderer::render(float partialTicks, const Camera& camera, const std::vector<Terrain*>& terrains)
{
	terrainShader.bind();
	terrainShader.u_SkyColor.uncheckedSet(glm::vec3(159/255.f, 191/255.f, 244/255.f));
	terrainShader.u_LightsUsed.uncheckedSet(lights.size());

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		terrainShader.u_LightPos[i].uncheckedSet(lights[i].getPos());
		terrainShader.u_PointLights[i].color.uncheckedSet(lights[i].getColor());
		terrainShader.u_PointLights[i].attenuation.uncheckedSet(lights[i].getAttenuation());
	}

	terrainShader.u_TransformationMatrix.uncheckedSet(glm::mat4(1.0));

	for (const Terrain* terrain : terrains)
	{
		const GlModel& model = terrain->getTerrainModel();
		prepareForRendering(*terrain);
		renderInstance(partialTicks, *terrain, camera);
	}
}


void TerrainRenderer::prepareForRendering(const Terrain& terrain)
{
	terrain.getTerrainModel().vao.bind();

	unsigned int lastBoundTexture = 0;

	for (unsigned int i = 0; i < terrain.getTextures().size(); i++) {
		terrainShader.u_Textures[i].uncheckedSet(lastBoundTexture);
		terrain.getTextures()[i].texture.promisedFetch().bind(lastBoundTexture++);
	}

	for (unsigned int i = 0; i < terrain.getTextures().size(); i++) {
		const TerrainTextureInfo& textureInfo = terrain.getTextures()[i];
		if (textureInfo.hasSpecularMap()) {
			terrainShader.u_SpecularMap[i].uncheckedSet(lastBoundTexture);
			textureInfo.specularMap->promisedFetch().bind(lastBoundTexture++);
		}

		terrainShader.u_HasSpecularMap[i].uncheckedSet(textureInfo.hasSpecularMap());
		terrainShader.u_Reflectivity[i].uncheckedSet(textureInfo.reflectivity);
		terrainShader.u_ShineDistanceDamper[i].uncheckedSet(textureInfo.shineDistanceDamper);
	}

	for (unsigned int i = 0; i < terrain.getBlendMaps().size(); i++) {
		terrainShader.u_BlendMap[i].uncheckedSet(lastBoundTexture);
		terrain.getBlendMaps()[i].bind(lastBoundTexture++);
	}
	
	terrainShader.u_TexturesUsed.uncheckedSet(terrain.getTextures().size());
}

void TerrainRenderer::renderInstance(float partialTicks, const Terrain& object, const Camera& camera)
{
	GlCall(glDrawElements(GL_TRIANGLES, object.getTerrainModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}