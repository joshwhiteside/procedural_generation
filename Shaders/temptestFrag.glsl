#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D secondTex;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

out vec4 gl_FragColor;


void main(void)	{

	gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	
}