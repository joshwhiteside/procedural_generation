#version 150 core

in vec2 texCoord;
in vec3	position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void) {

	OUT.texCoord = texCoord;

	mat4 mvp	= projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position,1.0);
}