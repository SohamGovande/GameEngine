#include <array>
#include "Shader.h"
#include "Uniform.t.h"
#include "Renderer/PPE/BlurAxis.h"


template<BlurAxis A>
class GaussianAxisShader
	: public Shader
{

public:
	//Vertex shader uniforms
	Uniform<float> u_PixelDensity;

	//Fragment shader uniforms
	Uniform<int> u_ScreenSampler;



public:
	inline GaussianAxisShader()
		: Shader(
			"gaussian_blur/vertex.glsl", "gaussian_blur/fragment.glsl",
			{ { "BLUR_DIRECTION", A == BlurAxis::HORIZONTAL ? "HORIZONTAL" : "VERTICAL" } },
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
		u_PixelDensity.loadLocation("u_PixelDensity", programID);

		//Fragment shader uniforms
		u_ScreenSampler.loadLocation("u_ScreenSampler", programID);


	}
};
