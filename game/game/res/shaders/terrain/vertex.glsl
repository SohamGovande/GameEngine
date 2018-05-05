#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_ToLightSource;
out vec3 v_ToCamera;
out float v_Visibility;

uniform mat4 u_TransformationMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_LightPos;

const float density = 0.003;
const float gradient = 1.5;

void main(void)
{
	//Position of the object without having the camera transformations applied
	vec4 positionWorldSpace = u_TransformationMatrix * vec4(position, 1.0);

	//Position of the object having the camera's viewpoint
	vec4 positionCameraSpace = u_ViewMatrix * positionWorldSpace;

	gl_Position = u_ProjectionMatrix * positionCameraSpace;

	v_TexCoord = texCoord;

	v_SurfaceNormal = (u_TransformationMatrix * vec4(normal, 0)).xyz;
	v_ToLightSource = u_LightPos - positionWorldSpace.xyz;

	/*
	We have to invert the view matrix because it stores the camera transformations
	in reverse, then we convert it to a vec4 by multiplying it by 0,0,0,1 and obtain
	its xyz position. 
	*/
	v_ToCamera = (inverse(u_ViewMatrix) * vec4(0, 0, 0, 1)).xyz - positionWorldSpace.xyz;

	float distance = length(positionCameraSpace.xyz);
	v_Visibility = clamp(exp(-pow(distance * density, gradient)), 0, 1);
}
