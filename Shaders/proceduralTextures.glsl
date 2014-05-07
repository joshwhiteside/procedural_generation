#version 150

uniform sampler3D noiseTex;



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


vec4 getMarble() {

	vec4 retColour;
	
	
	
	float temp = texture(noiseTex, vec3(IN.worldPos.xyz * 0.01)).r;
	
	temp = temp * temp;
	
	vec4 red = vec4(1.0,0.0,0.0,1.0);
	vec4 blue = vec4(0.0,0.0,1.0,1.0);
	
	retColour = mix(red,blue,temp);
	
	

	return retColour;
}

vec4 getMarble2() {

	vec4 retColour;
	
	vec3 samplePoint = IN.worldPos * 0.01;
	
	samplePoint.z = sin(samplePoint.z);
	
	float temp = texture(noiseTex, vec3(samplePoint)).r;
	
	temp = temp * temp;
	
	vec4 red = vec4(1.0,0.0,0.0,1.0);
	vec4 blue = vec4(0.0,0.0,1.0,1.0);
	
	retColour = mix(red,blue,temp);
	
	retColour.rgb = hueSwitch(retColour.rgb, temp);
	
	

	return retColour;
}