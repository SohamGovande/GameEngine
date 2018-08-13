#version 330 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_ScreenSampler;

void main()
{
	color = vec4(texture(u_ScreenSampler, v_TexCoord).rgb, 1.0);
}