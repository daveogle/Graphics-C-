// Minimal vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model_view, projection;
uniform uint colourmode;
uniform uint lightmode;
uniform float ambient, shininess;
uniform mat3 normal_matrix;
uniform vec3 light_dir;
uniform bool emit;
uniform vec3 specular_colour = vec3(0.7);

// Output the vertex colour - to be rasterized into pixel fragments
	out vec4 fcolour;

void main()
{
 vec4 emisive = vec4(0.0, 0.0, 0.0, 0.0);
	if(emit)
		emisive = vec4(1.0, 1.0, 1.0, 1.0);
	
	//multiply the normals by the normal transformation matrix then normalize
	vec3 N = normal_matrix * normal;
	vec4 P = vec4(model_view * vec4(position, 1.0));
	vec3 L = light_dir - P.xyz;
	vec3 V = -P.xyz;

	N = normalize(N);
	L = normalize(L);
	V = normalize(V);

	vec4 diffuse_colour;
	vec4 position_h = vec4(position, 1.0);
	
	//find diffuse component
	float diffuse_component = max(0.0, dot(N, L));
	
	vec3 H = normalize(L + V);	//find half vector

	vec4 specular = vec4(pow(max(dot(N, H), 0.0), shininess) * specular_colour, 1.0);

	if (colourmode == 0)
		diffuse_colour = colour;
	else
		diffuse_colour = vec4(1.0, 0.0, 0.0, 1.0);

	// Define the vertex colour

	if(lightmode == 0)
	{
		fcolour = diffuse_colour + ambient + emisive;
	}
	else if (lightmode == 1)
	{
		diffuse_colour = diffuse_component * diffuse_colour;
		fcolour = diffuse_colour + ambient + emisive;
	}
	else if (lightmode == 2)
	{
		fcolour = specular + diffuse_colour + ambient + emisive;
	}

	// Define the vertex position
	gl_Position = projection * model_view * position_h;
}