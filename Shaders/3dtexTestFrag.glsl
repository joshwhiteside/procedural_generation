#version 150 core

uniform sampler3D threeDTex;


in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

out vec4 gl_FragColor;


void main(void)	{

	vec4 tempcolour = texture(threeDTex, vec3(IN.texCoord.xy,0.5)).rrra;

	//tempcolour = vec4(IN.texCoord,0.0,1.0);
	tempcolour.a = 1.0;
	gl_FragColor = tempcolour;
	
}