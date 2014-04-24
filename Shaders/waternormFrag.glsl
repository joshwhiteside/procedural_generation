#version 150 core
uniform vec2 pixelSize;
uniform sampler2D juliaTex;

out vec4 gl_FragColor;

in Vertex {
	vec2 texCoord;
} IN;

uniform float time;


float getWaveHeight(vec2 uv, float tod) {
	vec2 position = uv;
	//position *= 512 * 16;
	//position *= 512;
	//position *= 16;
	
	float y = (sin((position.x * 3.142 + (tod*3))) + 
			   sin((position.y * 3.142 + (tod*3))))
				* 6;   
	
	return y;
}

void main(void) {
	
	vec4 output = vec4(0.0);
	float timeofday = sin(time*3.142);
	
	

	vec2 values[6];
	
	values = vec2[] (vec2(0.0, 0.0),
					 vec2(pixelSize.x, 0.0),
					 vec2(-pixelSize.x,0.0),
					 vec2(0.0, 0.0),
					 vec2(0.0, pixelSize.y),
					 vec2(0.0,-pixelSize.y)
					);
					
	float heights[6];
	
					
	for(int i = 0; i < 3; i++) {
		heights[i] = 0;
		heights[i+3] = 0;
		//if(timeofday < 0.5)
		//{
			heights[i] -= texture2D(juliaTex, IN.texCoord.xy + values[i]).r * 3;
			heights[i] += texture2D(juliaTex, IN.texCoord.xy + values[i]).g * 3;
			heights[i+3] -= texture2D(juliaTex, IN.texCoord.xy + values[i+3]).r * 3;
			heights[i+3] += texture2D(juliaTex, IN.texCoord.xy + values[i+3]).g * 3;
		//}
		heights[i]+= getWaveHeight( IN.texCoord.xy + values[i], timeofday);
		heights[i+3]+= getWaveHeight( IN.texCoord.xy + values[i], timeofday);
	}
	
	
	
	vec3 N = vec3(heights[1]-heights[0], heights[2]-heights[0], 0.1);
	vec3 N2 = vec3(heights[5]-heights[3], heights[4]-heights[3], 0.1);
	
	//output.rgb = normalize(N+N2);
	output.rgb = normalize(N2);
	
	gl_FragColor = output;
	//output = vec4(timeofday,0.0,0.0,1.0);
	//output =  texture2D(juliaTex, IN.texCoord.xy);
	//gl_FragColor.r = IN.texCoord.x;
	//gl_FragColor.g = IN.texCoord.y;
	//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	
	//gl_FragColor[1].rgb = texture2D(diffuseTex, IN.texCoord).rgb;
	//gl_FragColor[1].r = IN.texCoord.x;
	//gl_FragColor[1].b = IN.texCoord.y;
	//gl_FragColor[1] = vec4(0.0,0.0,1.0,1.0);
	
	//
	//gl_FragColor[1] = vec4(0.0,1.0,0.0,1.0);
}