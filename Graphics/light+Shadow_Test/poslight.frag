// Fragment shader

#version 420

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} fs_in;

uniform vec3 specular_colour, emissive, global_ambient, diffuse_colour;
uniform float shininess, ambient;
uniform sampler2D tex1;

out vec4 outputColor;

vec4 getCalculateColour(vec3 Light)
{
	float attenuation = 1.0;
	float distanceToLight = length(Light);
	float attenuation_k = 1.0f;
	attenuation = 2.0 / (attenuation_k * pow(distanceToLight, 2));

	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(Light);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);

	vec3 ambient_colour = diffuse_colour * ambient;
	vec3 diffuse = max(0.0, dot(N, L)) * diffuse_colour;
	vec3 specular = pow(max(dot(N, H), 0.0), shininess) * specular_colour;
	return vec4((attenuation*(ambient + diffuse + specular) + emissive + global_ambient), 1.0);
}

void main()
{	
	vec4 combinedLighting = vec4(0.0f);
	combinedLighting += getCalculateColour(fs_in.L);
	outputColor = combinedLighting;
}