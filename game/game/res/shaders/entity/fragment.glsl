#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightSource;
in vec3 v_ToCamera;
in float v_Visibility;

uniform sampler2D u_Texture;

uniform sampler2D u_SpecularMap;
uniform bool u_HasSpecularMap;

uniform float u_ShineDistanceDamper;
uniform float u_Reflectivity;
uniform vec3 u_LightColor;

uniform vec3 u_SkyColor;

vec3 calculateSpecular(vec3 unitSurfaceNorm, vec3 unitLightVec)
{
	vec3 specular = vec3(0, 0, 0);
	if (u_Reflectivity != 0)
	{
		vec3 unitVecToCamera = normalize(v_ToCamera);
		vec3 lightToVertex = -unitLightVec;
		vec3 reflectedDirection = reflect(lightToVertex, unitSurfaceNorm);

		float specularDot = dot(reflectedDirection, unitVecToCamera);
		specularDot = max(specularDot, 0.5);

		specular = pow(specularDot, u_ShineDistanceDamper) * u_Reflectivity * u_LightColor;

		//If there is specular map data, multiply the specular value by the map's red component (amount of shinyness to the material)
		if (u_HasSpecularMap)
			specular *= texture(u_SpecularMap, v_TexCoord).r;
	}
	return specular;
}

vec3 calculateDiffuse(vec3 unitSurfaceNorm, vec3 unitLightVec)
{
	//Find the difference between the vector to the light source and the surface nromal
	float diffuseDot = dot(unitSurfaceNorm, unitLightVec);

	diffuseDot = max(diffuseDot * 0.6 + .4, 0); //Ambient lighting

	return diffuseDot * u_LightColor;
}

void main(void)
{

	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (texColor.a < 0.7)
		discard;

	//Change the lengths of the vectors to be 1 and then find the dot product for diffuse lighting
	vec3 unitSurfaceNorm = normalize(v_SurfaceNormal);
	vec3 unitLightVec = normalize(v_ToLightSource);

	vec3 specular = calculateSpecular(unitSurfaceNorm, unitLightVec);
	vec3 diffuse = calculateDiffuse(unitSurfaceNorm, unitLightVec);

	color =	vec4(diffuse, 1) * texColor + vec4(specular, 1);
	color = mix(vec4(u_SkyColor.xyz, 1.0), color, v_Visibility);
}