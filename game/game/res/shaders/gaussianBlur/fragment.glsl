#version 330 core

in vec2 v_BlurTexCoords[11];

out vec4 color;

uniform sampler2D u_ScreenSampler;

const float gaussianWeights[11] = float[]( 0.0093, 0.028002, 0.065984, 0.121703, 0.175713, 0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093 );

void main()
{
	color = vec4(0);
	for (int i = 0; i < 11; i++)
		color += texture(u_ScreenSampler, v_BlurTexCoords[i]) * gaussianWeights[i];
}