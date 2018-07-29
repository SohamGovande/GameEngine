#include <array>
#include "Shader.h"
#include "Uniform.t.h"
#include "../Light.h"
#include "Terrain/TerrainConstants.h"

class TerrainShader
	: public Shader
{

public:
	//Vertex shader uniforms
	Uniform<glm::mat4> u_TransformationMatrix;
	Uniform<glm::mat4> u_ProjectionMatrix;
	Uniform<glm::mat4> u_ViewMatrix;
	std::array<Uniform<glm::vec3>, MAX_LIGHTS> u_LightPos;

	//Fragment shader uniforms
	std::array<Uniform<int>, MAX_TERRAIN_TEXTURES> u_Textures;
	Uniform<int> u_TexturesUsed;
	std::array<Uniform<int>, MAX_TERRAIN_TEXTURES> u_SpecularMap;
	std::array<Uniform<bool>, MAX_TERRAIN_TEXTURES> u_HasSpecularMap;
	std::array<Uniform<float>, MAX_TERRAIN_TEXTURES> u_ShineDistanceDamper;
	std::array<Uniform<float>, MAX_TERRAIN_TEXTURES> u_Reflectivity;
	std::array<Uniform<int>, MAX_TERRAIN_TEXTURES/4 + ((MAX_TERRAIN_TEXTURES % 4 == 0) ? 1 : 0)> u_BlendMap;
	Uniform<glm::vec3> u_SkyColor;
	std::array<Uniform<glm::vec3>, MAX_LIGHTS> u_LightColor;
	std::array<Uniform<glm::vec3>, MAX_LIGHTS> u_LightAttenuation;
	Uniform<int> u_LightsUsed;



public:
	inline TerrainShader()
		: Shader(
			"terrain/vertex.glsl", "terrain/fragment.glsl",
			{ { "MAX_LIGHTS", MAX_LIGHTS_STR } },
			{ { "MAX_LIGHTS", MAX_LIGHTS_STR }, { "MAX_TERRAIN_TEXTURES", MAX_TERRAIN_TEXTURES_STR } }
		)
	{
		loadUniformLocations();
	}

private:
	inline void loadUniformLocations()
	{
		//We need the GL program ID to load uniform locations for that program.
		const GLuint programID = rendererID;

		//Vertex shader uniforms
		u_TransformationMatrix.loadLocation("u_TransformationMatrix", programID);
		u_ProjectionMatrix.loadLocation("u_ProjectionMatrix", programID);
		u_ViewMatrix.loadLocation("u_ViewMatrix", programID);
		for(unsigned int i = 0; i < (MAX_LIGHTS); i++)
			u_LightPos[i].loadLocation("u_LightPos[" + std::to_string(i) + "]", programID);

		//Fragment shader uniforms
		for(unsigned int i = 0; i < (MAX_TERRAIN_TEXTURES); i++)
			u_Textures[i].loadLocation("u_Textures[" + std::to_string(i) + "]", programID);
		u_TexturesUsed.loadLocation("u_TexturesUsed", programID);
		for(unsigned int i = 0; i < (MAX_TERRAIN_TEXTURES); i++)
			u_SpecularMap[i].loadLocation("u_SpecularMap[" + std::to_string(i) + "]", programID);
		for(unsigned int i = 0; i < (MAX_TERRAIN_TEXTURES); i++)
			u_HasSpecularMap[i].loadLocation("u_HasSpecularMap[" + std::to_string(i) + "]", programID);
		for(unsigned int i = 0; i < (MAX_TERRAIN_TEXTURES); i++)
			u_ShineDistanceDamper[i].loadLocation("u_ShineDistanceDamper[" + std::to_string(i) + "]", programID);
		for(unsigned int i = 0; i < (MAX_TERRAIN_TEXTURES); i++)
			u_Reflectivity[i].loadLocation("u_Reflectivity[" + std::to_string(i) + "]", programID);
		for(unsigned int i = 0; i < (MAX_TERRAIN_TEXTURES/4 + ((MAX_TERRAIN_TEXTURES % 4 == 0) ? 1 : 0)); i++)
			u_BlendMap[i].loadLocation("u_BlendMap[" + std::to_string(i) + "]", programID);
		u_SkyColor.loadLocation("u_SkyColor", programID);
		for(unsigned int i = 0; i < (MAX_LIGHTS); i++)
			u_LightColor[i].loadLocation("u_LightColor[" + std::to_string(i) + "]", programID);
		for(unsigned int i = 0; i < (MAX_LIGHTS); i++)
			u_LightAttenuation[i].loadLocation("u_LightAttenuation[" + std::to_string(i) + "]", programID);
		u_LightsUsed.loadLocation("u_LightsUsed", programID);


	}
};
