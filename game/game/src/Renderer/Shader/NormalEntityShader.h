#include <array>
#include "Shader.h"
#include "Uniform.t.h"
#include "../Light.h"


class NormalEntityShader
	: public Shader
{

public:
	//Vertex shader uniforms
	Uniform<glm::mat4> u_TransformationMatrix;
	Uniform<glm::mat4> u_ProjectionMatrix;
	Uniform<glm::mat4> u_ViewMatrix;
	std::array<Uniform<glm::vec3>, MAX_LIGHTS> u_LightPos;

	//Fragment shader uniforms
	Uniform<int> u_Texture;
	Uniform<int> u_SpecularMap;
	Uniform<bool> u_HasSpecularMap;
	Uniform<float> u_ShineDistanceDamper;
	Uniform<float> u_Reflectivity;
	Uniform<glm::vec3> u_SkyColor;
	std::array<Uniform<glm::vec3>, MAX_LIGHTS> u_LightColor;
	std::array<Uniform<glm::vec3>, MAX_LIGHTS> u_LightAttenuation;
	Uniform<int> u_LightsUsed;



public:
	inline NormalEntityShader()
		: Shader(
			"entity/vertex.glsl", "entity/fragment.glsl",
			{ { "MAX_LIGHTS", MAX_LIGHTS_STR } },
			{ { "MAX_LIGHTS", MAX_LIGHTS_STR } }
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
		u_Texture.loadLocation("u_Texture", programID);
		u_SpecularMap.loadLocation("u_SpecularMap", programID);
		u_HasSpecularMap.loadLocation("u_HasSpecularMap", programID);
		u_ShineDistanceDamper.loadLocation("u_ShineDistanceDamper", programID);
		u_Reflectivity.loadLocation("u_Reflectivity", programID);
		u_SkyColor.loadLocation("u_SkyColor", programID);
		for(unsigned int i = 0; i < (MAX_LIGHTS); i++)
			u_LightColor[i].loadLocation("u_LightColor[" + std::to_string(i) + "]", programID);
		for(unsigned int i = 0; i < (MAX_LIGHTS); i++)
			u_LightAttenuation[i].loadLocation("u_LightAttenuation[" + std::to_string(i) + "]", programID);
		u_LightsUsed.loadLocation("u_LightsUsed", programID);


	}
};
