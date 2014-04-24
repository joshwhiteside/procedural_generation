#version 150 core

uniform sampler2D diffuseTex;
uniform vec2	  pixelSize;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 gl_FragColor;

void main(void)		{
	float d = texture2D(diffuseTex, IN.texCoord.xy).r;
	
	gl_FragColor = vec4(d,d,d,1.0);
	
}