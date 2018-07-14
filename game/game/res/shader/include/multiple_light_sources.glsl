#ifdef LIGHTING_UNIFORMS_IN_TANGENT_SPACE
#define UNIFORM_toLightSource v_TangentToLightSource
#else
#define UNIFORM_toLightSource v_ToLightSource
#endif

void calculateLighting(in vec3 unitVecToCamera, in vec3 unitSurfaceNorm, in vec2 texCoord, out vec3 diffuse, out vec3 specular)
{
	for (int i = 0; i < u_LightsUsed; i++)
	{
		float lightDistance = length(UNIFORM_toLightSource[i]);
		vec3 unitLightVec = UNIFORM_toLightSource[i] / lightDistance;

		float attenuation = 1.0 / (lightDistance * lightDistance * u_LightAttenuation[i].z + lightDistance * u_LightAttenuation[i].y + u_LightAttenuation[i].x);

		diffuse += calculateDiffuse(unitSurfaceNorm, unitLightVec) * attenuation * u_LightColor[i];
		if (PHONG_LIGHTING_reflectivity != 0)
			specular += calculateSpecular(unitSurfaceNorm, unitLightVec, texCoord, unitVecToCamera
#ifdef PHONG_LIGHTING_USE_SPECULAR_ARRAYS
, i
#endif
				) * attenuation * u_LightColor[i];
	}
}
