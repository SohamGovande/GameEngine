#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightSource[MAX_LIGHTS];
in vec3 v_ToCamera;
in float v_Visibility;

uniform sampler2D u_Texture;

uniform sampler2D u_SpecularMap;
uniform bool u_HasSpecularMap;

uniform float u_ShineDistanceDamper;
uniform float u_Reflectivity;

uniform vec3 u_SkyColor;

uniform vec3 u_LightColor[MAX_LIGHTS];
uniform vec3 u_LightAttenuation[MAX_LIGHTS];
uniform int u_LightsUsed;

#define PI 3.1416

#include <phong_lighting.glsl>
#include <multiple_light_sources.glsl>

void main(void)
{

	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (texColor.a < 0.7)
		discard;

	vec3 unitSurfaceNorm = normalize(v_SurfaceNormal);
	vec3 unitVecToCamera = normalize(v_ToCamera);

	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);
	calculateLighting(unitVecToCamera, unitSurfaceNorm, v_TexCoord, diffuse, specular);
	
	diffuse = max(diffuse, 0.2);

	color =	vec4(diffuse, 1) * texColor + vec4(specular, 1);
	color = mix(vec4(u_SkyColor.xyz, 1.0), color, v_Visibility);
}