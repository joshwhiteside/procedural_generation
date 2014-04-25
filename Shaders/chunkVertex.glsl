#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform sampler3D threeDTex;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
	vec3 worldPos;
	float particleSize;
} OUT;

void main(void)	{

	OUT.colour = colour;
	OUT.position = position;
	OUT.texCoord = (textureMatrix * vec4(texCoord,0.0,1.0)).xy;
	
	vec3 worldPos = ( modelMatrix * vec4 ( position ,1)). xyz ;
	
	OUT.worldPos = worldPos;
	
	OUT.particleSize = (texture(threeDTex, worldPos* 0.01).r);
	//OUT.particleSize = 0.1;
 gl_Position = ( projMatrix * viewMatrix * modelMatrix ) * vec4 ( position , 1.0);

}