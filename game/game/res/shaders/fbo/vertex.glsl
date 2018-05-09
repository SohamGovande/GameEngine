#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = vec4(pos.xy, 0, 1);
}