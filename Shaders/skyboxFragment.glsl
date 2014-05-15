#version 400 core

uniform sampler2D	diffuseTex;		//NOISE
uniform vec3		cameraPos;
uniform float		time;

uniform float		dayRatio;//

uniform vec3 lightPos;

uniform sampler3D noiseTex;

in Vertex	{
	vec3 normal;
} IN;

out vec4 gl_FragColor;


uniform vec4 auroraColour2= vec4(0.0,0.0,1.0,1.0);
uniform vec4 auroraColour = vec4(0.0,1.0,0.0,1.0);


int max_iterations = 20;




vec4 getColour(vec3 i) {
	vec4 value1 = vec4(1.0,0.0,1.0,1.0);		
	return value1;
}

vec3 hueSwitch(vec3 c, float shift) {
	vec3 retValue;
	
	//Converts RGB to HSV
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    vec3 hsv = vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
	
	//Changes the hue
	hsv.x += shift;
	
	//Converts back to RGB
	K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p2 = abs(fract(hsv.xxx + K.xyz) * 6.0 - K.www);
    vec3 temp =   vec3(hsv.z * mix(K.xxx, clamp(p2 - K.xxx, 0.0, 1.0), hsv.y));
	retValue = temp.rgb;
	
	return retValue;
}

vec4 getAurora(vec2 z) {
	vec4 outputcolour = auroraColour;
	
	int iterations = 0;
	
	vec2 original = z;
	
	float threshold_squared = 2.0 + (sin (time * 0.0075) + sin(time * 0.01)) * 0.05;
	
	while(iterations < max_iterations && dot(z,z) < threshold_squared){
	
	
		vec2 z_squared;
		z_squared.x = (z.x * z.x) - (z.y * z.y);
		z_squared.y = 2.0 * z.x * z.y;		
		z = sin(z_squared) + 0.5;
		
		iterations++;
	}
	if(iterations == max_iterations) {
		outputcolour = vec4(1.0,0.0,0.0,0.0);
		
		return outputcolour;
	} else {
	
		float f = iterations;
		f /= max_iterations;
	
		
		outputcolour = mix(auroraColour,auroraColour2, f);
	}

	/*
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p2 = abs(fract(outputcolour.xxx + K.xyz) * 6.0 - K.www);
    vec3 temp =   vec3(outputcolour.z * mix(K.xxx, clamp(p2 - K.xxx, 0.0, 1.0), outputcolour.y));
	outputcolour.rgb = temp.rgb;
	*/
	return outputcolour;
}



void main(void){
	gl_FragColor	 = getAurora(vec2(IN.normal.x,IN.normal.y+IN.normal.z));
	
	

	gl_FragColor.rgb = hueSwitch(gl_FragColor.rgb,texture(noiseTex, vec3(IN.normal.xyz + (time * 0.001))) );
//	 
	gl_FragColor.a = 1.0;
}