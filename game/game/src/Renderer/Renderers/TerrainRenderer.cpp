#include "../GlMacros.h"
#include "TerrainRenderer.h"
#include "MathUtils.h"
#include "MasterRenderer.h"

TerrainRenderer::TerrainRenderer(MasterRenderer& masterRenderer, const std::vector<Light>& lights, Shader& shader)
	: lights(lights), shader(shader), masterRenderer(masterRenderer)
{
	
}
void TerrainRenderer::render(float partialTicks, const Camera& camera, const std::vector<Terrain*>& terrains)
{
	shader.bind();

	shader.setFloat("u_Reflectivity", 0);
	shader.setVec3("u_SkyColor", 176 / 255.f, 231 / 255.f, 232 / 255.f);

	shader.setInt("u_LightsUsed", lights.size());
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		std::string iString = std::to_string(i);
		shader.setVec3("u_LightPos[" + iString + "]", lights[i].getPos().x, lights[i].getPos().y, lights[i].getPos().z);
		shader.setVec3("u_LightColor[" + iString + "]", lights[i].getColor().r, lights[i].getColor().g, lights[i].getColor().b);
		shader.setFloat("u_LightAttenuation[" + iString + "]", lights[i].getAttenuation());
		shader.setFloat("u_LightBrightness[" + iString + "]", lights[i].getBrightness());
	}

	shader.setMat4("u_TransformationMatrix", glm::mat4(1.0f));
	shader.setMat4("u_ViewMatrix", Math::createViewMatrix(camera));

	for (const Terrain* terrain : terrains)
	{
		const GlModel& model = terrain->getTerrainModel();
		prepareForRendering(*terrain);
		renderInstance(partialTicks, *terrain, camera);
	}
}


void TerrainRenderer::prepareForRendering(const Terrain& terrain) const
{
	terrain.getTerrainModel().vao.bind();

	unsigned int lastBoundTexture = 0;

	for (unsigned int i = 0; i < terrain.getTextures().size(); i++) {
		shader.setInt("u_Textures[" + std::to_string(i) + "]", lastBoundTexture);
		terrain.getTextures()[i].texture.promisedFetch().bind(lastBoundTexture++);
	}

	for (unsigned int i = 0; i < terrain.getTextures().size(); i++) {
		const TerrainTextureInfo& textureInfo = terrain.getTextures()[i];
		const std::string iString = std::to_string(i);

		if (textureInfo.hasSpecularMap()) {
			shader.setInt("u_SpecularMap[" + iString + "]", lastBoundTexture);
			textureInfo.specularMap->promisedFetch().bind(lastBoundTexture++);
		}
		shader.setInt("u_HasSpecularMap[" + iString + "]", textureInfo.hasSpecularMap());
		shader.setFloat("u_Reflectivity", textureInfo.reflectivity);
		shader.setFloat("u_ShineDistanceDamper", textureInfo.shineDistanceDamper);
	}

	for (unsigned int i = 0; i < terrain.getBlendMaps().size(); i++) {
		shader.setInt("u_BlendMap[" + std::to_string(i) + "]", lastBoundTexture);
		terrain.getBlendMaps()[i].bind(lastBoundTexture++);
	}

	shader.setInt("u_TexturesUsed", terrain.getTextures().size());
}

void TerrainRenderer::renderInstance(float partialTicks, const Terrain& object, const Camera& camera)
{
	GlCall(glDrawElements(GL_TRIANGLES, object.getTerrainModel().ibo.getCount(), GL_UNSIGNED_INT, 0));
}