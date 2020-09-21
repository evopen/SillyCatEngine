#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

struct SMVP
{
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

layout(binding = 5) uniform MVP_Block {
	SMVP MVP;
};

layout(location = 0) out vec3 fragColor;

void main()
{
	gl_Position = MVP.Projection * MVP.View * MVP.Model * vec4(inPosition, 1);
	fragColor = inColor.xyz;
}