#version 150 core

uniform sampler2D	diffuseTex;
uniform int			useTexture;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
}	IN;

out vec4 gl_FragColor;

void main(void)		{
	if(IN.colour == vec4(1,0,1,1)) {
		//IN.colour.a -=1.0f;
		IN.colour.a =0.4f;
		//gl_FragColor = (IN.colour.xyz,0.5f);
	}

	gl_FragColor = IN.colour;
	if(useTexture > 0)	{
		gl_FragColor *= texture(diffuseTex, IN.texCoord);
	}
}