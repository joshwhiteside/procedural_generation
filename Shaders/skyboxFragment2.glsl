#version 400 core

uniform sampler2D	diffuseTex;		//NOISE
uniform vec3		cameraPos;
uniform float		time;

uniform float		dayRatio;//

uniform vec3 lightPos;

in Vertex	{
	vec3 normal;
} IN;

out vec4 gl_FragColor;

//uniform vec4 cloudColour = vec4(1.0,0.7,0.7,1.0);
uniform vec4 cloudColour = vec4(.7,0.7,0.7,1.0);

uniform vec4 auroraColour2= vec4(0.27,1.0,1.0,1.0);
uniform vec4 auroraColour = vec4(0.88,1.0,1.0,1.0);

bool isDay = false;


float cloudSharpness = 0.0;
float cloudCover 	 = 1.0;

float sunRadius		 = 0.289;
vec3  sunPos		 = normalize(vec3(0.5,0.0,0.5));

int max_iterations = 20;


vec3 getNoiseVal(vec2 input) {
	vec2 i = input;
	i += 1.0;
		i *= 0.5;
	
	return texture2D(diffuseTex, input).rgb;

}

vec4 getCloud(vec2 input) {
		vec2 i = input;
		i += 1.0;
		i *= 0.5;	
		//i*= 4;
		i += vec2(time * 0.00005);
		vec4 retValue;
		
		float temp =texture2D(diffuseTex, i).r;
		//temp 
		retValue = vec4(temp);
		float cloudCheck = 1-cloudCover;
		
		
		
		retValue *= cloudColour;
		return retValue;
}

vec4 getAuroraShape(vec2 input) {
		vec2 i = input;
		i += 1.0;
		i *= 0.5;	
		i += vec2((cos(sin(time * 0.0005) * 0.25)));
		i += getNoiseVal(input).r;
		vec4 retValue;
		
		float temp =texture2D(diffuseTex, i).r;
		//temp 
		retValue = vec4(temp);
		
		retValue *= 0.5;
		return retValue;
}

//FRACTAL
vec4 getAurora(vec2 z) {
	vec4 outputcolour = auroraColour;
	
	int iterations = 0;
	
	vec2 original = z;
	
	z+= vec2(getNoiseVal(original).r);
	
	float threshold_squared = 4.0 +  + (sin (time * 0.0075) + sin(time * 0.001)) * 0.5;
	
	while(iterations < max_iterations && dot(z,z) < threshold_squared){
	
	
		vec2 z_squared;
		z_squared.x = (z.x * z.x) - (z.y * z.y);
		z_squared.y = 2.0 * z.x * z.y;		
		z = sin(z_squared) + 0.5; // sin(time * 0.00001)
		
		iterations++;
	}
	if(iterations == max_iterations) {
		outputcolour = vec4(0.0,0.0,0.0,0.0);
		
		return outputcolour;
	} else {
	
		float f = iterations;
		f /= max_iterations;
	
		
		outputcolour = mix(auroraColour,auroraColour2, f);
	}

	outputcolour.x *= getCloud(z.yx);
	
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p2 = abs(fract(outputcolour.xxx + K.xyz) * 6.0 - K.www);
    vec3 temp =   vec3(outputcolour.z * mix(K.xxx, clamp(p2 - K.xxx, 0.0, 1.0), outputcolour.y));
	outputcolour.rgb = temp.rgb;
	
	return outputcolour;
}

vec3 getAuroraCol(vec2 input) {
		vec3 retValue = vec3(0.0);

		vec2 i = input;
		i += 1.0;
		i *= 100;
		float starVal = texture2D(diffuseTex, i).r;
		
		retValue.r = sin(i.x) * sin(i.x);
		retValue.g = sin(i.y) * sin(i.y);
		retValue.b = cos(i.y) * cos(i.y);
		
		retValue *= retValue;
			
		
		return retValue; 
}

vec3 getStar(vec2 input) {
		vec3 retValue = vec3(0.0);

		vec2 i = input;
		i += 1.0;
		i *= 100;
		float starVal = texture2D(diffuseTex, i).r;
		
		if(starVal>0.5 && starVal < 0.51) {
			retValue = vec3(0.5,0.5,1.0);
		}
		
		
		return retValue; 
}


vec3 getSunPos() {
	vec3 retValue = lightPos - cameraPos;
	return normalize(retValue.rgb);
}

float getSunInfluence() {
	vec3 pos = getSunPos();
	vec3 sunSpace = ( IN.normal + 1) * 0.5;
	if( length(cross(IN.normal, pos)) < 0.05) {

		return  1.0;
	
	}

	return 0.0;
}



vec4 getColour(vec3 i) {
	vec4 value1 = vec4(1.0,0.0,1.0,1.0);
	
	float skyVal = IN.normal.y;
	skyVal += 1.0;
	skyVal *= 0.5;
	skyVal = 1 - skyVal;
	
	//daytime
	vec4 daytime = vec4(skyVal,0.5+(skyVal*0.5),1.0,1.0);
	//sunset
	vec4 sunset= vec4(0.992,0.37,0.33,1.0);
	//sunrise
	vec4 sunrise = vec4(skyVal,0.5+ (skyVal * 0.5),0.9,1.0);
	//nighttime
	vec4 nighttime = vec4((skyVal * 0.25), (skyVal * 0.5),(skyVal * 0.5),1.0);  
	
	
	float val = sin(time * 0.0001) * cos(time * 0.0001);
	
	
	if(dayRatio > 0.5) isDay = true;
	
	value1  = mix(nighttime, daytime, dayRatio);

	if(dayRatio > 0.5)value1 += vec4(getSunInfluence());
	if(dayRatio < 0.4){
		//value1.rgb  +=  mix(getAurora(vec2(i.x * i.z ,i.x + i.y)).rgb ,  getAuroraCol(vec2(i.x * i.z * i.x, i.y * i.y * i.z) * 0.05 + val).rgb,0.05) *  getAuroraShape(vec2(i.y * i.x, i.y * i.z)).rgb * ((0.3 - dayRatio) / 0.3);
		value1.rgb += getAurora(vec2(i.x * i.z, i.x + i.y)).rgb * (((1-dayRatio) - 0.6) * 2.5);
	}
	vec4 cloud;		
	cloud= getCloud(vec2(i.x , i.y * i.z) * 2);
	cloud.a *= cloud.a * cloud.a;
	//else cloud.a = 1.;
	
	value1 = mix(value1,cloud,cloud.a);     
		
	return value1;
}


void main(void){
	gl_FragColor	 = getColour(normalize(IN.normal));
//	gl_FragColor.rgb = ( IN.normal + 1) * 0.5;
//	 
	gl_FragColor.a = 1.0;
}