#version 330 core
#define HORIZONTAL 1
#define VERTICAL 2
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 v_BlurTexCoords[11];

uniform float u_PixelDensity;

void main()
{
	gl_Position = vec4(pos.xy, 0, 1);

	for (int i = -5; i <= 5; i++)
	{
		v_BlurTexCoords[i + 5] = texCoord +
#if BLUR_DIRECTION == HORIZONTAL
			vec2(i * u_PixelDensity, 0)
#elif BLUR_DIRECTION == VERTICAL
			vec2(0, i * u_PixelDensity)
#else
#error "Invalid blur direction or none specified"
#endif
		;
	}
}