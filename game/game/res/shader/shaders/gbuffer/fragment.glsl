#version 330

in vec3 v_FragPos;
in vec2 v_TexCoord;
in vec3 v_Normal;

layout(location = 0) out vec3 gbufPosition;
layout(location = 1) out vec3 gbufNormal;
layout(location = 2) out vec4 gbufAlbedoSpec;

uniform sampler2D u_DiffuseSampler;

void main(void)
{
	gbufPosition = v_FragPos;
	gbufNormal = normalize(v_Normal);
	gbufAlbedoSpec.rgb = texture(u_DiffuseSampler, v_TexCoord).rgb;
	gbufAlbedoSpec.a = 0;
}