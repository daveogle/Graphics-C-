//Vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 texcoord;

// Uniform variables are passed in from the application
uniform mat4 model_view, projection;
uniform mat3 normal_matrix;
uniform vec3 light_pos;
uniform uint numberOfLights, texture_mode;

// Output the vertex colour - to be rasterized into pixel fragments
out VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} vs_out;


void main()
{
	vec4 P = model_view * vec4(position, 1.0f);
	vs_out.N = normal_matrix * normal;
	vs_out.L = light_pos - P.xyz;
	vs_out.V = -P.xyz;

	gl_Position = projection * P;
}