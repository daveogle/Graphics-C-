// Fragment shader

#version 420

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} fs_in;

uniform vec3 specular_colour, diffuse_colour, emissive, global_ambient;
uniform float shininess, ambient;

out vec4 outputColor;

void main()
{
	float distanceToLight = length(fs_in.L);
	float attenuation_k = 2.0;
    float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2));

	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);

	vec3 ambient_colour = diffuse_colour * ambient;
	vec3 diffuse = max(0.0, dot(N, L)) * diffuse_colour;
	vec3 specular = pow(max(dot(N, H), 0.0), shininess) * specular_colour;

	outputColor = vec4(attenuation*(ambient + diffuse + specular) + emissive + global_ambient, 1.0);
}