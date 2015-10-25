//Vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec4 position;

// Uniform variables are passed in from the application


// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;


void main()
{
	vec4 colour = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = position;
	fcolour = colour;
}