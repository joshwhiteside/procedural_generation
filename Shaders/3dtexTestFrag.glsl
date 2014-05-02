#version 150 core

uniform sampler3D threeDTex;
uniform float timer;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

out vec4 gl_FragColor;


void main(void)	{

	
	vec4 col1 = vec4(1.0,0.0,1.0,1.0);
	vec4 col2 = vec4(0.0,1.0,1.0,1.0);
	vec4 tempcolour = texture(threeDTex, vec3(IN.texCoord.xy,timer)).rrra;

	//float mixVal =  texture(threeDTex, vec3(IN.texCoord.xy,timer)).r;
	//tempcolour = mix(col1,col2,mixVal);
	//tempcolour = vec4(IN.texCoord,0.0,1.0);
	tempcolour.a = 1.0;
	gl_FragColor = tempcolour;
	
}