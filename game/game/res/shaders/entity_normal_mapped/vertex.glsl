#version 330 core

layout(location = 0) in vec3 vbo_Position;
layout(location = 1) in vec2 vbo_TexCoord;
layout(location = 2) in vec3 vbo_Normal;
layout(location = 3) in vec3 vbo_Tangent;

out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_TangentToLightSource[MAX_LIGHTS];
out vec3 v_TangentToCamera;
out float v_Visibility;

uniform mat4 u_TransformationMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_LightPos[MAX_LIGHTS];

const float density = 0.002;
const float gradient = 2;

void main(void)
{
	mat4 modelViewProjection = u_ProjectionMatrix * u_ViewMatrix * u_TransformationMatrix;

	//Position of the object without having the camera transformations applied
	vec4 positionWorldSpace = u_TransformationMatrix * vec4(vbo_Position, 1.0);
	//Position of the object having the camera's viewpoint
	vec4 positionCameraSpace = u_ViewMatrix * positionWorldSpace;

	gl_Position = u_ProjectionMatrix * positionCameraSpace;

	v_TexCoord = vbo_TexCoord;
	v_SurfaceNormal = (u_TransformationMatrix * vec4(vbo_Normal, 0)).xyz;

	mat3 normalMatrix = transpose(inverse(mat3(u_TransformationMatrix)));
	vec3 T = normalize(normalMatrix * vbo_Tangent);
	vec3 N = normalize(normalMatrix * vbo_Normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);

	mat3 TBN = transpose(mat3(T, B, N));

	for (int i = 0; i < MAX_LIGHTS; i++)
		v_TangentToLightSource[i] = TBN * (u_LightPos[i] - positionWorldSpace.xyz);

	/*
	We have to invert the view matrix because it stores the camera transformations
	in reverse, then we convert it to a vec4 by multiplying it by 0,0,0,1 and obtain
	its xyz position. 
	*/
	vec3 cameraPos = (inverse(u_ViewMatrix) * vec4(0, 0, 0, 1)).xyz;
	v_TangentToCamera = TBN * (cameraPos - positionWorldSpace.xyz);

	float distance = length(positionCameraSpace.xyz);
	v_Visibility = clamp(exp(-pow(distance * density, gradient)), 0, 1);
}
