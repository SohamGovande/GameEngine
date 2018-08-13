#include <array>
#include "Shader.h"
#include "Uniform.t.h"
#include "../Light.h"


class IdentityShader
	: public Shader
{

public:
	//Fragment shader uniforms
	Uniform<int> u_ScreenSampler;



public:
	inline IdentityShader()
		: Shader(
			"identity/vertex.glsl", "identity/fragment.glsl",
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

		//Fragment shader uniforms
		u_ScreenSampler.loadLocation("u_ScreenSampler", programID);


	}
};
