#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightSource[MAX_LIGHTS];
in vec3 v_ToCamera;
in float v_Visibility;

struct PointLight
{
	vec3 color;
	vec3 attenuation;
};

uniform sampler2D u_Textures[MAX_TERRAIN_TEXTURES];
uniform int u_TexturesUsed;

uniform sampler2D u_SpecularMap[MAX_TERRAIN_TEXTURES];
uniform bool u_HasSpecularMap[MAX_TERRAIN_TEXTURES];
uniform float u_ShineDistanceDamper[MAX_TERRAIN_TEXTURES];
uniform float u_Reflectivity[MAX_TERRAIN_TEXTURES];
uniform sampler2D u_BlendMap[MAX_TERRAIN_TEXTURES/4 + ((MAX_TERRAIN_TEXTURES % 4 == 0) ? 1 : 0)];

uniform vec3 u_SkyColor;

uniform PointLight u_PointLights[MAX_LIGHTS];
uniform int u_LightsUsed;

#define PI 3.1416

#define PHONG_LIGHTING_USE_SPECULAR_ARRAYS
#include <phong_lighting.glsl>
#include <multiple_light_sources.glsl>

float getBlendmapValue(in int index) {
	switch (index % 4)
	{
		case 0:
			return texture(u_BlendMap[index / 4], v_TexCoord).r;
		case 1:
			return texture(u_BlendMap[index / 4], v_TexCoord).g;
		case 2:		
			return texture(u_BlendMap[index / 4], v_TexCoord).b;
		default: /*case 3:*/
			return texture(u_BlendMap[index / 4], v_TexCoord).a;
	}
}

void main(void)
{

	vec4 texColor = vec4(0);
	for (int i = 0; i < u_TexturesUsed; i++) {
		texColor += texture(u_Textures[i], v_TexCoord * 10) * getBlendmapValue(i);
	}
	if (texColor.a < 0.7)
		discard;

	vec3 unitSurfaceNorm = normalize(v_SurfaceNormal);
	vec3 unitVecToCamera = normalize(v_ToCamera);

	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);
	
	calculateLighting(unitVecToCamera, unitSurfaceNorm, v_TexCoord * 10, diffuse, specular);
	diffuse = max(diffuse, 0.2);

	color =	vec4(diffuse, 1) * texColor + vec4(specular, 1);

    vec4 grayscale = vec4(vec3(dot(color.rgb, vec3(0.2125, 0.7154, 0.0721))), 1.0);

    color = mix(grayscale, color, 1.25);
	color = mix(vec4(u_SkyColor.xyz, 1.0), color, v_Visibility);
}