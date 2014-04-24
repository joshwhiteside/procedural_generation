#version 150 core

uniform sampler2D diffuseTex; //Used in this case for the depth buffer
uniform sampler2D secondTex; 
uniform vec2 pixelSize;


 uniform sampler2D bumpTex; //Used in this case for the original image.

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

out vec4 gl_FragColor;

	
float linearizeDepth(vec2 uv) {
	float n = 1.0;
	float f = 10000.0;
	float z = texture(diffuseTex, uv).r;
	return (2.0*n) /(f+n -z * (f-n));
}


const float weights[3] = float[](20,20,20);

void main(void)	{
	float depth = linearizeDepth(IN.texCoord);
	vec4 image = texture(bumpTex, IN.texCoord).rgba;

		
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
	
	
	float V;
	float H;

	for(int i = 0; i<3; i++) {
		float tmp = linearizeDepth(IN.texCoord.xy+vertValues[i]);
		 V += tmp * weights[i];	
	
		tmp = linearizeDepth(IN.texCoord.xy+horizValues[i]);
		H += tmp * weights[i];
	}
	for(int i = 0; i<3; i++) {
		float tmp = linearizeDepth(IN.texCoord.xy+vertValues[3+i]);
		V -= tmp * weights[i];
		
		tmp = linearizeDepth(IN.texCoord.xy+horizValues[3+i]);
		H -= tmp * weights[i];
	}

	//

	vec2 check 		  = vec2(-pixelSize.x, 0.0 );
	float depthcheck  = linearizeDepth(check);

	
	vec4 sobelAdd = vec4(sqrt((V*V) + (H*H)));
	
	sobelAdd = vec4(1)-sobelAdd;
	sobelAdd.a = 1.0f;
	
	gl_FragColor = sobelAdd * image;
	
	//gl_FragColor.a = 1.0;
	
	
	
	
	
	/*
	
	depthcheck -= depth;
	if(depthcheck < 0) depthcheck *= -1;
	

	if(depthcheck < 0.001) {
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0f);
	} else {
	//gl_FragColor = mix(outputcolour,image, 0.5);
	gl_FragColor = image;
	}
	
	*/
	
}






/*
	DEPTH COLOURATION!
	vec4 outputcolour;
	
	if (depth<0.90) {
	outputcolour = vec4(0.0,1.0,1.0,1.0f);}
	else if (depth<0.95){
	outputcolour = vec4(1.0,1.0,1.0,1.0f);
	}
	else if (depth<0.98){
	outputcolour = vec4(1.0,0.0,0.0,1.0f);
	}	
	else if (depth<0.99){
	outputcolour = vec4(1.0,1.0,0.0,1.0f);
	}
	else if (depth<0.995){
	outputcolour = vec4(1.0,0.0,1.0,1.0f);
	}
	else if (depth<0.999){
	outputcolour = vec4(0.8,0.3,0.8,1.0f);
	}
	
	else {
	outputcolour = vec4(0.0,1.0,0.0,1.0f);
	}
	
	
	
	ORIGINAL
		
	vec2 checks[4];
	checks = vec2[] (
						vec2(pixelSize.x, 0.0),
						vec2(pixelSize.x, 0.0), 
						vec2(0.0,  pixelSize.x),
						vec2(0.0,  pixelSize.x) 
				   );
	bool isOutline =true;
	float depthcheck;
	float depthval = 0;
	for(int i = 0; i < 4; i++) {
		float depthcheck  = texture(diffuseTex, (checks[i])).r;
		depthcheck -= depth;
		if(depthcheck < 0) depthcheck *= -1;
		depthcheck = sqrt(depthcheck);
		
		//depthval+=depthcheck;
		if(!(depthcheck < 0.001)) {isOutline = false;}
	}
	//vec2 checker = vec2(IN.texCoord.x, IN.texCoord.y + pixelSize.y);
	//float depthval  = texture(diffuseTex, (checker)).r;
	depthval/=10;
	
	
	if (isOutline) {
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0f);
	} else {
	//gl_FragColor = mix(outputcolour,image, 0.5);
	gl_FragColor = image;
	}
	
	

*/