//Vertex shader

#version 400

#define MAX_LIGHTS 2 //arrays must have a constant value

// These are the vertex attributes
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;

// Uniform variables are passed in from the application
uniform mat4 model_view, projection;
uniform mat3 normal_matrix;
uniform vec3 light_pos[MAX_LIGHTS];
uniform uint numberOfLights;

// Output the vertex colour - to be rasterized into pixel fragments
out VS_OUT
{
	vec3 N;
	vec3 L[MAX_LIGHTS];
	vec3 V;
	vec3 diffuse_colour;
} vs_out;


void main()
{
	vec4 P = model_view * position;
	vs_out.N = normal_matrix * normal;

	for(int i = 0; i < numberOfLights; i++)
	{
		vs_out.L[i] = light_pos[i] - P.xyz;
	}
	vs_out.V = -P.xyz;
	vs_out.diffuse_colour = colour;

	gl_Position = projection * P;
}