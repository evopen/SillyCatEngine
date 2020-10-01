#version 450

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoords;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D BaseColor;

void main()
{
	outColor = texture(BaseColor, inTexCoords);
}