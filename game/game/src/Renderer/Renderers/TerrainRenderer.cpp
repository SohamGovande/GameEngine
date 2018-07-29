#include "../GlMacros.h"
#include "TerrainRenderer.h"
#include "MathUtils.h"
#include "MasterRenderer.h"

TerrainRenderer::TerrainRenderer(MasterRenderer& masterRenderer, const std::vector<Light>& lights, const glm::mat4& projectionMatrix, const std::string& maxLightsStr)
	: lights(lights),
	terrainShader(),
	masterRenderer(masterRenderer)
{
	terrainShader.bind();
	terrainShader.u_ProjectionMatrix.setAndUpdate(projectionMatrix);
}

TerrainRenderer::~TerrainRenderer()
{
}

void TerrainRenderer::render(float partialTicks, const Camera& camera, const std::vector<Terrain*>& terrains)
{
	terrainShader.bind();
	terrainShader.u_SkyColor.update();
	terrainShader.u_LightsUsed.setAndUpdate(lights.size());

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		terrainShader.u_LightPos[i].setAndUpdate(lights[i].getPos());
		terrainShader.u_LightColor[i].setAndUpdate(lights[i].getColor());
		terrainShader.u_LightAttenuation[i].setAndUpdate(lights[i].getAttenuation());
	}

	terrainShader.u_TransformationMatrix.setAndUpdate(glm::mat4(1.0));
	terrainShader.u_ViewMatrix.setAndUpdate(Math::createViewMatrix(camera));

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
		terrainShader.u_Textures[i].setAndUpdate(lastBoundTexture);
		terrain.getTextures()[i].texture.promisedFetch().bind(lastBoundTexture++);
	}

	for (unsigned int i = 0; i < terrain.getTextures().size(); i++) {
		const TerrainTextureInfo& textureInfo = terrain.getTextures()[i];
		const std::string iString = std::to_string(i);

		if (textureInfo.hasSpecularMap()) {
			terrainShader.u_SpecularMap[i].setAndUpdate(lastBoundTexture);
			textureInfo.specularMap->promisedFetch().bind(lastBoundTexture++);
		}

		terrainShader.u_HasSpecularMap[i].setAndUpdate(textureInfo.hasSpecularMap());
		terrainShader.u_Reflectivity[i].setAndUpdate(textureInfo.reflectivity);
		terrainShader.u_ShineDistanceDamper[i].setAndUpdate(textureInfo.shineDistanceDamper);
	}

	for (unsigned int i = 0; i < terrain.getBlendMaps().size(); i++) {
		terrainShader.u_BlendMap[i].setAndUpdate(lastBoundTexture);
		terrain.getBlendMaps()[i].bind(lastBoundTexture++);
	}
	
	terrainShader.u_TexturesUsed.setAndUpdate(terrain.getTextures().size());
}

void TerrainRenderer::renderInstance(float partialTicks, const Terrain& object, const Camera& camera)
{
	GlCall(glDrawElements(GL_TRIANGLES, object.getTerrainModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}