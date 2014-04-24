#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D originalImage;


uniform vec2	  pixelSize;
uniform int		  isVertical;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 gl_FragColor;

const float weights[3] = float[](-1,8,-1);
//const float weights[5] = float[](0.12,0.22,0.32,0.22,0.12);


void main(void)		{
	//Edge detection!
	
	vec2 horizValues[6];
	vec2 vertValues[6];
	

	horizValues = vec2[](  	
								vec2(-pixelSize.x, -pixelSize.y*2),
								vec2(-pixelSize.x , -pixelSize.y*1), 
								vec2(-pixelSize.x , 0.0),
								vec2(pixelSize.x  , 0.0), 
								vec2(pixelSize.x , pixelSize.y*1),
								vec2(pixelSize.x , pixelSize.y*2));
	
	
	vertValues = vec2[](		vec2(-pixelSize.x*2, -pixelSize.y),
								vec2(-pixelSize.x*1 , -pixelSize.y),
								vec2( 0.0 , -pixelSize.y),
								vec2( 0.0 ,pixelSize.y),
								vec2(pixelSize.x , pixelSize.y),
								vec2(pixelSize.x*2 ,pixelSize.y)) ;
	
	
	vec4 V;
	vec4 H;
	
	
	for(int i = 0; i<3; i++) {
		vec4 tmp = texture2D(diffuseTex, IN.texCoord.xy + vertValues[i]);
		 V += tmp * weights[i];	
	
		tmp = texture2D(diffuseTex, IN.texCoord.xy + horizValues[i]);
		H += tmp * weights[i];
	}
	for(int i = 0; i<3; i++) {
		vec4 tmp = texture2D(diffuseTex, IN.texCoord.xy + vertValues[3+i]);
		V -= tmp * weights[i];
		
		tmp = texture2D(diffuseTex, IN.texCoord.xy + horizValues[3+i]);
		H -= tmp * weights[i];
		
	
	}
	
	vec4 orig = texture2D(originalImage, IN.texCoord.xy);
	
	vec4 sobelAdd = sqrt((V*V) + (H*H));
	
	//gl_FragColor = mix(sobelAdd,orig, 0.5);
	gl_FragColor = sobelAdd;
	
	gl_FragColor.a = 1.0;
	
	
	
	
	
	
}