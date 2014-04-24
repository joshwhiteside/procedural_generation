#version 150 core

uniform sampler2D colourTex;
uniform sampler2D depthTex;
uniform sampler2D normalTex;

uniform vec2 pixelSize;

uniform samplerCube	cubeTex;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
	vec3 sbnormal;
} IN;

out vec4 gl_FragColor;

float linearizeDepth(vec2 uv) {
	float n = 1.0;
	float f = 10000.0;
	float z = texture(depthTex, uv).r;
	return (2.0*n) /(f+n -z * (f-n));
}

float normalDifference(vec3 normal, vec2 uv) {
	vec3 nearnormal = texture(normalTex, uv).rgb;
	
	float f = dot(nearnormal, normal);
	f = f + 1;
	f = f * 0.5;
	
	return f;
}





const float weights[3] = float[](1,1,1);

void main(void)	{
	float depth = linearizeDepth(IN.texCoord);
	vec4 image = texture2D(colourTex, IN.texCoord).rgba;
	float pureDepth = texture2D(depthTex, IN.texCoord).r;
	//vec3 skynormal = normalize(sbnormal);
	
	
	//Depth of field here.
	
	vec2 values[8];
	
	values = vec2[] (vec2(-pixelSize.x, -pixelSize.y),
					 vec2(0.0, -pixelSize.y),
					 vec2(pixelSize.x, -pixelSize.y),
					 vec2(-pixelSize.x, 0.0),
					 vec2(pixelSize.x, 0.0),
					 vec2(-pixelSize.x, pixelSize.y),
					 vec2(0.0, pixelSize.y),
					 vec2(pixelSize.x, pixelSize.y)
					);
					
	for(int i = 0; i < 8; i++) {
		vec4 tempcolour = texture2D(colourTex, IN.texCoord.xy + values[i]);
		float tempdepth = linearizeDepth(IN.texCoord.xy + values[i]);
		//float tempdepth = 1;
		gl_FragColor += tempcolour * tempdepth;
	
	}
	
	//gl_FragColor = mix(image, gl_FragColor, depth);
	
	//Comic shading here. (DEPTH BUFFER)	
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


//Comic shader with depths
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




//Normal checker

	vec3 thisnormal =texture2D(normalTex, IN.texCoord).rgb;

	for(int i = 0; i<3; i++) {
		float tmp = normalDifference(thisnormal,IN.texCoord.xy+vertValues[i]);
		 V += tmp * weights[i];	
	
		tmp = normalDifference(thisnormal,IN.texCoord.xy+horizValues[i]);
		H += tmp * weights[i];
	}
	for(int i = 0; i<3; i++) {
		float tmp = normalDifference(thisnormal,IN.texCoord.xy+vertValues[3+i]);
		V -= tmp * weights[i];
		
		tmp = normalDifference(thisnormal,IN.texCoord.xy+horizValues[3+i]);
		H -= tmp * weights[i];
	}



	vec2 check 		  = vec2(-pixelSize.x, 0.0 );
	float depthcheck  = linearizeDepth(check);

	
	vec4 sobelAdd = vec4(sqrt((V*V) + (H*H)));
	sobelAdd*=1.5;
	sobelAdd = vec4(1)-sobelAdd;
	sobelAdd.a = 1.0f;
	/*
	if(sobelAdd.r>0.5){
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	} 
	else {
		gl_FragColor = image;
	}
	*/
	if(sobelAdd.r>0.5) sobelAdd =vec4(1.0);
	else sobelAdd = vec4(0.0);
	
	gl_FragColor *= sobelAdd * image;
	
	//gl_FragColor = texture2D(normalTex, IN.texCoord);
	
	if(pureDepth ==1.0) {
	}
	
	gl_FragColor.a = 1.0;
}