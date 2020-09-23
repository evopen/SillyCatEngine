#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outColor;

struct S_MVP
{
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

layout(set = 0, binding = 0) uniform MVP_Block {
	S_MVP MVP;
};

void main()
{
	gl_Position = MVP.Projection * MVP.View * MVP.Model * vec4(inPosition, 1.0);
	outColor = inColor;
}