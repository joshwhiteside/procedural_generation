#version 150 core

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 skyMatrix;
in vec3 position;

out Vertex	{
	vec3 normal;
} OUT;

void main(void){
	vec3 tempPos = position;
	OUT.normal	 = transpose(mat3(viewMatrix)) * normalize(tempPos);
	//OUT.normal	 = inverse(mat3(viewMatrix)) * normalize(tempPos);
	gl_Position	 = skyMatrix * projMatrix * vec4(tempPos, 1.0);
}