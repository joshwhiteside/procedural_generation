#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D secondTex;

uniform float time;

uniform vec4 unicolour;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

out vec4 gl_FragColor[2];


void main(void)	{

	float timeofday = time;
	if(timeofday < 0) timeofday *=-1;
	
	float dissolve = texture(diffuseTex, IN.texCoord).r;
	if(timeofday<dissolve) discard;
	
	
	//tempcolour  +=  IN.colour*vec4(0.01f);
	gl_FragColor[0] = unicolour;
	
	//gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
	
	//gl_FragColor = vec4(fract(gl_FragCoord.y), 0, 0, 1);
	//gl_FragColor = vec4(checkvalue,checkvalue,checkvalue,1.0f);
	//if (gl_FragColor.x<0.1 || gl_FragColor.y<0.1 || gl_FragColor.z<0.1)  discard;
	
	gl_FragColor[1] = vec4(0.0,1.0,0.0,1.0);
}