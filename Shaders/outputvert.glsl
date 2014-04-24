#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 sbMatrix;
uniform mat4 pjMatrix;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
	vec3 sbnormal;
} OUT;

void main(void)	{
	mat4 mvp	= projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position,1.0);
	OUT.colour = colour;
	OUT.position = position;
	OUT.texCoord = (textureMatrix * vec4(texCoord,0.0,1.0)).xy;
	
	
	vec3 tempPos = position - vec3(0,0,1);
	tempPos.y *= -1;
	OUT.sbnormal	 = transpose(mat3(sbMatrix * pjMatrix)) * normalize(tempPos);
	//OUT.sbnormal	= mat3(sbMatrix) * normalize(tempPos);
	//OUT.sbnormal = inverse(mat3(sbMatrix)) * normalize(tempPos);
}