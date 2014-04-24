#version 150 core

uniform sampler2D diffuseTex;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 gl_FragColor[2];

void main(void)	{
	vec4 outcolour = texture(diffuseTex, IN.texCoord);
	if (outcolour.a==0.0) discard;
	gl_FragColor[0] =outcolour;
	//gl_FragColor.a = 1.0;
}