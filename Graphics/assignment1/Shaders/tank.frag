// Fragment shader

#version 420

#define MAX_LIGHTS 2 //arrays must have a constant value

in VS_OUT
{
	vec3 N;
	vec3 L[MAX_LIGHTS];
	vec3 V;
	vec3 diffuse_colour;
	vec2 ftexcoord;
} fs_in;

uniform vec3 specular_colour, emissive, global_ambient;
uniform float shininess, ambient;
uniform uint light_mode, numberOfLights;
uniform sampler2D tex1;

out vec4 outputColor;

vec4 getCalculateColour(vec3 Light)
{
	vec4 texcolour = texture(tex1, fs_in.ftexcoord);
	float attenuation = 1.0;
	float distanceToLight = length(Light);
	float attenuation_k = 1.0f;
	if(light_mode == 1)
	{
		attenuation = 1.0 / (attenuation_k * pow(distanceToLight, 2));
	}

	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(Light);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);

	vec3 ambient_colour = fs_in.diffuse_colour * ambient;
	vec3 diffuse = max(0.0, dot(N, L)) * fs_in.diffuse_colour;
	vec3 specular = pow(max(dot(N, H), 0.0), shininess) * specular_colour;

	vec4 colour = vec4((attenuation*(ambient + diffuse + specular) + emissive + global_ambient), 1.0);
	return colour * texcolour;
}

void main()
{	
	vec4 combinedLighting = vec4(0.0f);
	for(int i = 0; i < numberOfLights; i++)
	{
		combinedLighting += getCalculateColour(fs_in.L[i]);
	}
	outputColor = combinedLighting;
}