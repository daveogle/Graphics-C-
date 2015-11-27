// shadow vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec4 position;

// Uniform variables are passed in from the application
uniform mat4 model_view, projection;

void main()
{
	vec4 P = model_view * position;
	gl_Position = projection * P;
}

