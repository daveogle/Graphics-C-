// Fragment shader

#version 420

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	vec3 diffuse_colour;
} fs_in;

uniform vec3 specular_colour, emissive, global_ambient;
uniform float shininess, ambient;
uniform uint light_mode;

out vec4 outputColor;

void main()
{
	float attenuation = 1.0;
	float distanceToLight = length(fs_in.L);
	float attenuation_k = 1.0f;
	if(light_mode == 1)
	{
		attenuation = 1.0 / (attenuation_k * pow(distanceToLight, 2));
	}

	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);

	vec3 ambient_colour = fs_in.diffuse_colour * ambient;
	vec3 diffuse = max(0.0, dot(N, L)) * fs_in.diffuse_colour;
	vec3 specular = pow(max(dot(N, H), 0.0), shininess) * specular_colour;

	outputColor = vec4((attenuation*(ambient + diffuse + specular) + emissive + global_ambient), 1.0);
}