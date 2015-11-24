// Minimal fragment shader

#version 420

in vec4 fcolour;
out vec4 outputColor;

uniform sampler2D snowTex;

void main()
{
	vec4 texcolour = texture(snowTex, gl_PointCoord);

	/* Discard the white colours and turn the black white*/
	//if (texcolour.r > 0.9 && texcolour.g > 0.9 && texcolour.b > 0.9) 
		//discard;
	if(texcolour.r < 0.1 && texcolour.g < 0.1 && texcolour.b < 0.1)
		discard;//texcolour.r = texcolour.g = texcolour.b = 1.0;

	outputColor = fcolour * texcolour;
}