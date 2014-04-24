#version 150 core

uniform sampler2D	diffuseTex;
uniform int			useTexture;

uniform vec3 		cubeManWorldPosition;
uniform float 		minDistance;
uniform float 		maxDistance;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec4 worldSpacePosition;
}	IN;

out vec4 gl_FragColor;

void main(void)		{


	float dist = distance(IN.worldSpacePosition.xyz, cubeManWorldPosition);
	
	
	float calcDist = dist - minDistance;
	calcDist = calcDist/maxDistance;
	
	calcDist = clamp(calcDist, 0.0, 1.0);
	calcDist = 1 - calcDist;
	
	
	if(IN.colour == vec4(1,0,1,1)) {
		//IN.colour.a -=1.0f;
		IN.colour.a =0.4f;
		//calcDist += 0.4f;
		//gl_FragColor = (IN.colour.xyz,0.5f);
	}

	gl_FragColor = IN.colour;
	gl_FragColor.a = calcDist;
	
	//gl_FragColor = vec4(calcDist,calcDist,calcDist,1);
	if(useTexture > 0)	{
		gl_FragColor *= texture(diffuseTex, IN.texCoord);
	}
	
}