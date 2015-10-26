//Vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model_view, projection;
uniform float shininess;
uniform mat3 normal_matrix;
uniform vec3 light_dir;
uniform vec3 specular_colour, ambient_colour, diffuse_colour;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;


void main()
{
	vec3 N = normal_matrix * normal;
	vec4 P = model_view * position;
	vec3 L = light_dir - P.xyz;
	vec3 V = -P.xyz;

	N = normalize(N);
	L = normalize(L);
	V = normalize(V);

	//find diffuse component
	vec4 diffuse = vec4(max(0.0, dot(N, L)) * diffuse_colour, 1.0);

	//find half vector
	vec3 H = normalize(L + V);	

	vec4 specular = vec4(pow(max(dot(N, H), 0.0), shininess) * specular_colour, 1.0);

	fcolour = specular + diffuse + vec4(ambient_colour, 1.0);
	gl_Position = projection * model_view * position;
}