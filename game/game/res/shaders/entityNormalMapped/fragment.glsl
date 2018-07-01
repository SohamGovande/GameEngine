#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_TangentToLightSource[MAX_LIGHTS];
in vec3 v_TangentToCamera;
in float v_Visibility;

uniform sampler2D u_Texture;

uniform sampler2D u_SpecularMap;
uniform bool u_HasSpecularMap;

uniform sampler2D u_NormalMap;

uniform float u_ShineDistanceDamper;
uniform float u_Reflectivity;

uniform vec3 u_SkyColor;

uniform vec3 u_LightColor[MAX_LIGHTS];
uniform float u_LightAttenuation[MAX_LIGHTS];
uniform float u_LightBrightness[MAX_LIGHTS];
uniform int u_LightsUsed;

#define PI 3.1416

float calculateDiffuse(in vec3 unitSurfaceNorm, in vec3 unitLightVec)
{
	return max(dot(unitSurfaceNorm, unitLightVec), 0);
}

float calculateSpecular(in vec3 unitSurfaceNorm, in vec3 unitLightVec, in vec3 unitVecToCamera)
{
	float specularDot = max(dot(reflect(-unitLightVec, unitSurfaceNorm), unitVecToCamera), 0.5);
	float actualSpecular = pow(specularDot, u_ShineDistanceDamper) * u_Reflectivity;

	//If there is specular map data, multiply the specular value by the map's red component (amount of shinyness to the material)
	if (u_HasSpecularMap)
		actualSpecular *= texture(u_SpecularMap, v_TexCoord).r;

	return actualSpecular;
}

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
	
	for (int i = 0; i < u_LightsUsed; i++)
	{
		vec3 unitLightVec = normalize(v_TangentToLightSource[i]);

		float lightDistanceSq = v_TangentToLightSource[i].x*v_TangentToLightSource[i].x + v_TangentToLightSource[i].y*v_TangentToLightSource[i].y + v_TangentToLightSource[i].z*v_TangentToLightSource[i].z;
		//a = brightness/(1.0 + k*d^2)
		float attenuation =  u_LightBrightness[i] / (1.0 + u_LightAttenuation[i] * lightDistanceSq);

		diffuse += calculateDiffuse(unitSurfaceNorm, unitLightVec) * attenuation * u_LightColor[i];
		if (u_Reflectivity != 0)
			specular += calculateSpecular(unitSurfaceNorm, unitLightVec, unitVecToCamera) * attenuation * u_LightColor[i];
	}
	
	diffuse = max(diffuse, 0.2);

	color =	vec4(diffuse, 1) * texColor + vec4(specular, 1);
	color = mix(vec4(u_SkyColor.xyz, 1.0), color, v_Visibility);
}