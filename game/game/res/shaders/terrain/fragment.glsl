#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightSource[MAX_LIGHTS];
in vec3 v_ToCamera;
in float v_Visibility;

uniform sampler2D u_Textures[MAX_TERRAIN_TEXTURES];
uniform int u_TexturesUsed;

uniform sampler2D u_SpecularMap[MAX_TERRAIN_TEXTURES];
uniform bool u_HasSpecularMap[MAX_TERRAIN_TEXTURES];
uniform float u_ShineDistanceDamper[MAX_TERRAIN_TEXTURES];
uniform float u_Reflectivity[MAX_TERRAIN_TEXTURES];
uniform sampler2D u_BlendMap[MAX_TERRAIN_TEXTURES/4 + ((MAX_TERRAIN_TEXTURES % 4 == 0) ? 1 : 0)];

uniform vec3 u_SkyColor;

uniform vec3 u_LightColor[MAX_LIGHTS];
uniform vec3 u_LightAttenuation[MAX_LIGHTS];
uniform int u_LightsUsed;

#define PI 3.1416

float calculateDiffuse(in vec3 unitSurfaceNorm, in vec3 unitLightVec)
{
	return max(dot(unitSurfaceNorm, unitLightVec), 0);
}

float calculateSpecular(in vec3 unitSurfaceNorm, in vec3 unitLightVec, in vec3 unitVecToCamera, in int i)
{

	float specularDot = max(dot(reflect(-unitLightVec, unitSurfaceNorm), unitVecToCamera), 0.5);
	float actualSpecular = pow(specularDot, u_ShineDistanceDamper[i]) * u_Reflectivity[i];

	//If there is specular map data, multiply the specular value by the map's red component (amount of shinyness to the material)
	if (u_HasSpecularMap[i])
		actualSpecular *= texture(u_SpecularMap[i], v_TexCoord).r;

	return actualSpecular;
}

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
	if (texColor.a < 0.7) {
		discard;
		return;
	}

	vec3 unitSurfaceNorm = normalize(v_SurfaceNormal);
	vec3 unitVecToCamera = normalize(v_ToCamera);

	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);
	
	for (int i = 0; i < u_LightsUsed; i++)
	{
		float lightDistance = length(v_ToLightSource[i]);
		vec3 unitLightVec = v_ToLightSource[i] / lightDistance;

		float attenuation =  1.0 / (lightDistance * lightDistance * u_LightAttenuation[i].z + lightDistance * u_LightAttenuation[i].y + u_LightAttenuation[i].x);

		diffuse += calculateDiffuse(unitSurfaceNorm, unitLightVec) * attenuation * u_LightColor[i];

		for (int j = 0; j < u_TexturesUsed; j++) {
			if (u_Reflectivity[j] != 0)
				specular += calculateSpecular(unitSurfaceNorm, unitLightVec, unitVecToCamera, j) * attenuation * u_LightColor[i];
		}
	}
	
	diffuse = max(diffuse, 0.2);

	color =	vec4(diffuse, 1) * texColor + vec4(specular, 1);

    vec4 grayscale = vec4(vec3(dot(color.rgb, vec3(0.2125, 0.7154, 0.0721))), 1.0);

    color = mix(grayscale, color, 1.25);
	color = mix(vec4(u_SkyColor.xyz, 1.0), color, v_Visibility);
}