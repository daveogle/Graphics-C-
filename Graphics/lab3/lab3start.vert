// Minimal vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model_view, projection;
uniform uint colourmode;
uniform float ambient;
uniform mat3 normal_matrix;
uniform vec3 light_dir;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;

void main()
{
	//multiply the normals by the normal transformation matrix then normalize
	vec3 N = normal_matrix * normal;
	N = normalize(N);

	vec4 diffuse_colour;
	vec4 position_h = vec4(position, 1.0);
	
	//find diffuse component
	float diffuse_component = max(0.0, dot(N, light_dir));
	
	if (colourmode == 1)
		diffuse_colour = colour;
	else
		diffuse_colour = vec4(0.0, 1.0, 0, 1.0);

		//Define diffuse values
		diffuse_colour = diffuse_component * diffuse_colour;

	// Define the vertex colour
	fcolour = diffuse_colour * ambient;

	// Define the vertex position
	gl_Position = projection * model_view * position_h;
}

