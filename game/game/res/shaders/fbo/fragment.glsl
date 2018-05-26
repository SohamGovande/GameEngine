#version 330 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_ScreenSampler;

const float offset = 1.0 / 700.0;

void main()
{
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
	vec3 sky = vec3(176.0/255, 231.0/255, 232.0/255.f);

	int count = 0;
	for (int i = 0; i < 9; i++)
		if (texture(u_ScreenSampler, v_TexCoord + offsets[i]).xyz == sky.xyz)
			count++;

	if (count == 0 || count >= 9)
		color = texture(u_ScreenSampler, v_TexCoord);
	else if (count < 9)
		color = vec4(1, 1, 1, 1);
}