// Minimal fragment shader

#version 420

in vec4 fcolour;
in vec2 ftexcoord;
out vec4 outputColor;

layout (binding=0) uniform sampler2D tex1;
layout (binding=1) uniform sampler2D tex2;

void main()
{
	vec4 texcolour1 = texture(tex1, ftexcoord);
	vec4 texcolour2 = texture(tex2, ftexcoord);
	outputColor = fcolour * (texcolour1 * texcolour2);
}