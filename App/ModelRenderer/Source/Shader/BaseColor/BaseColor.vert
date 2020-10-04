#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoords;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;

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
	outTexCoords = inTexCoords;
}