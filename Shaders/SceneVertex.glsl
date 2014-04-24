#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColour;

in vec3 position;
in vec2 texCoord;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec4 worldSpacePosition;
} OUT;

void main(void)		{

	vec4 wsp = modelMatrix * vec4(position, 1.0);
	gl_Position		= (projMatrix * viewMatrix * wsp);
	
	
	OUT.worldSpacePosition = wsp;					
	OUT.texCoord	= texCoord;
	OUT.colour		= nodeColour;
}