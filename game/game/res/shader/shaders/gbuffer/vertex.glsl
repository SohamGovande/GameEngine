#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec3 v_FragPos;
out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 u_TransformationMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main(void)
{
	vec4 worldPos = u_TransformationMatrix * vec4(position, 1.0);
	v_FragPos = worldPos.xyz;
	v_TexCoord = texCoord;

	mat3 normalMatrix = transpose(inverse(mat3(u_TransformationMatrix)));
	v_Normal = normalMatrix * normal;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPos;
}