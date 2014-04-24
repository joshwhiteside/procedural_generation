#version 150 core

in vec3 position;
in vec4 colour;

out vec3 controlpoint_wor;

out Vertex {
	vec4 colour;
} OUT;

void main(void)		{
	controlpoint_wor = position;
	gl_Position = vec4(position,1.0);
	OUT.colour = colour;
}