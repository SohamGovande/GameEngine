#include <array>
#include "Shader.h"
#include "Uniform.t.h"
#include "../Light.h"


class GBufferGeometryPassShader
	: public Shader
{

public:
	//Vertex shader uniforms
	Uniform<glm::mat4> u_TransformationMatrix;
	Uniform<glm::mat4> u_ViewMatrix;
	Uniform<glm::mat4> u_ProjectionMatrix;

	//Fragment shader uniforms
	Uniform<int> u_DiffuseSampler;



public:
	inline GBufferGeometryPassShader()
		: Shader(
			"gbuffer/vertex.glsl", "gbuffer/fragment.glsl",
			{ },
			{ }
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
		u_ViewMatrix.loadLocation("u_ViewMatrix", programID);
		u_ProjectionMatrix.loadLocation("u_ProjectionMatrix", programID);

		//Fragment shader uniforms
		u_DiffuseSampler.loadLocation("u_DiffuseSampler", programID);


	}
};
