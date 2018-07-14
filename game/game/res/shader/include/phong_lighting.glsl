#ifdef PHONG_LIGHTING_USE_SPECULAR_ARRAYS

#define PHONG_LIGHTING_specularMap u_SpecularMap[i]
#define PHONG_LIGHTING_hasSpecularMap u_HasSpecularMap[i]
#define PHONG_LIGHTING_reflectivity u_Reflectivity[i]
#define PHONG_LIGHTING_shineDistanceDamper u_ShineDistanceDamper[i]

#else

#define PHONG_LIGHTING_specularMap u_SpecularMap
#define PHONG_LIGHTING_hasSpecularMap u_HasSpecularMap
#define PHONG_LIGHTING_reflectivity u_Reflectivity
#define PHONG_LIGHTING_shineDistanceDamper u_ShineDistanceDamper

#endif

float calculateDiffuse(in vec3 unitSurfaceNorm, in vec3 unitLightVec)
{
	return max(dot(unitSurfaceNorm, unitLightVec), 0);
}

float calculateSpecular(in vec3 unitSurfaceNorm, in vec3 unitLightVec, in vec2 texCoords, in vec3 unitVecToCamera
#ifdef PHONG_LIGHTING_USE_SPECULAR_ARRAYS
	, in int i
#endif
	)
{

	float specularDot = max(dot(reflect(-unitLightVec, unitSurfaceNorm), unitVecToCamera), 0.5);
	float actualSpecular = pow(specularDot, PHONG_LIGHTING_shineDistanceDamper) * PHONG_LIGHTING_reflectivity;

	//If there is specular map data, multiply the specular value by the map's red component (amount of shinyness to the material)
	if (PHONG_LIGHTING_hasSpecularMap)
		actualSpecular *= texture(PHONG_LIGHTING_specularMap, texCoords).r;

	return actualSpecular;
}