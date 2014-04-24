#version 150 core

uniform float		time;
uniform samplerCube	cubeTex;
uniform samplerCube	cubeTex2;
uniform vec3		cameraPos;

in Vertex	{
	vec3 normal;
} IN;

out vec4 gl_FragColor[2];

//out vec4 gl_FragColor;

void main(void){
	float timeofday = sin(time * 3.142);
	
	vec4 colour1 = texture(cubeTex, normalize(IN.normal));
	vec4 colour2 = texture(cubeTex2, normalize(IN.normal));
	
	
	gl_FragColor[0] = mix(colour2, colour1, timeofday);
	

	
	//gl_FragColor = vec4(0.0,1.0,0.0,1.0);
}