#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_TangentToLightSource[MAX_LIGHTS];
in vec3 v_TangentToCamera;
in float v_Visibility;


uniform sampler2D u_Texture;

uniform bool u_HasSpecularMap;
uniform sampler2D u_SpecularMap;
uniform float u_ShineDistanceDamper;
uniform float u_Reflectivity;

uniform sampler2D u_NormalMap;

uniform vec3 u_SkyColor;

uniform vec3 u_LightColor[MAX_LIGHTS];
uniform vec3 u_LightAttenuation[MAX_LIGHTS];
uniform int u_LightsUsed;

#define PI 3.1416

#define LIGHTING_UNIFORMS_IN_TANGENT_SPACE
#include <phong_lighting.glsl>
#include <multiple_light_sources.glsl>

void main(void)
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (texColor.a < 0.7)
		discard;

	vec3 unitSurfaceNorm = texture(u_NormalMap, v_TexCoord).rgb;
	unitSurfaceNorm = normalize(unitSurfaceNorm * 2.0 - 1.0);

	vec3 unitVecToCamera = normalize(v_TangentToCamera);

	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);
	
	calculateLighting(unitVecToCamera, unitSurfaceNorm, v_TexCoord, diffuse, specular);
	
	diffuse = max(diffuse, 0.2);

	color =	vec4(diffuse, 1) * texColor + vec4(specular, 1);
	color = mix(vec4(u_SkyColor.xyz, 1.0), color, v_Visibility);
}
