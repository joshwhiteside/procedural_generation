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

	vec4 tempcolour = texture(diffuseTex, IN.texCoord);
	
	if(tempcolour.r==0.0) discard;
	
	
	gl_FragColor[0] = tempcolour;
}