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
	vec4 tempcolour = texture(secondTex, IN.texCoord);
	//vec4 tempcolour += cos(IN.position.y)/10;
	//tempcolour+=vec4(0.5f);
	//tempcolour *=vec4(0.0f,1.0f,1.0f,1.0f);
	tempcolour.a = 0.5f;
	
	//tempcolour  *=  IN.colour;
	gl_FragColor = tempcolour;
}