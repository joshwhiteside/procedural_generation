#version 150 core

uniform sampler2D diffuseTex;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	gl_FragColor = IN.colour* texture(diffuseTex, IN.texCoord);
	if(gl_FragColor.a != 0.0) gl_FragColor.a = 1.0;
}
