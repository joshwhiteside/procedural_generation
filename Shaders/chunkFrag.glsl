
#version 150 core


 uniform vec3  cameraPos ;
 uniform vec4  lightColour = vec4(1.0,1.0,1.0,1.0);
 uniform vec3  lightPos    = vec3(16.0,64.0,16.0);
 uniform float lightRadius = 5000;

 float DRAW_SIZE = 10.0f;
 float TOP_Y = 32.0f;
 float MAX_HEIGHT = DRAW_SIZE * TOP_Y;
 

uniform sampler3D noiseTex;
uniform float timer;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
	vec3 worldPos;
	vec3 normal;
} IN;

out vec4 gl_FragColor;



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
	
	vec3 samplePoint = IN.worldPos * 0.1;
	//samplePoint.x = sin(samplePoint.x) + samplePoint.x;
	//samplePoint.z = sin(samplePoint.z) + samplePoint.z;
	
	float temp = texture(noiseTex, vec3(samplePoint)).r;
		
	vec4 red = vec4(1.0,0.5,0.0,1.0);
	vec4 blue = vec4(0.0,0.8,1.0,1.0);
	
	float temp2 = sin( IN.worldPos.y) + (temp * 0.95);
	
	retColour = mix(red,blue,temp2);
	
	//retColour.rgb = hueSwitch(retColour.rgb, temp);

	return retColour;
}


vec4 getBrightColour() {

	vec4 retColour;
	
	vec3 samplePoint = IN.worldPos * 0.01;
		
	float temp = texture(noiseTex, vec3(samplePoint)).r;
	
	temp = temp * temp;
	
	vec4 red = vec4(1.0,0.0,0.0,1.0);
	vec4 blue = vec4(0.0,0.0,1.0,1.0);
	
	retColour = mix(red,blue,temp);
	
	retColour.rgb = hueSwitch(retColour.rgb, temp);
	//retColour.rgb *= temp;
	

	return retColour;
}




vec4 getColour() {
	vec4 retColour;

	float temp = IN.worldPos.y;
	temp = IN.worldPos.y / MAX_HEIGHT;
	temp *= temp;
	
	retColour = mix(getMarble(),getBrightColour(), temp);
	//retColour = getMarble();
	
	return retColour;
}


void main(void)	{

	vec4 diffuse =getColour();//= vec4(IN.colour);
	vec3 incident = normalize ( lightPos - IN.worldPos );
	float lambert = max (0.0 , dot ( incident , IN.normal ));

	
	
  float dist = length ( lightPos - IN.worldPos );
  float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

  vec3 viewDir = normalize ( cameraPos - IN.worldPos );
  vec3 halfDir = normalize ( incident + viewDir );
  
  float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
  float sFactor = pow ( rFactor , 50.0 );


  vec3 colour = ( diffuse.rgb * lightColour.rgb );
  colour += ( lightColour.rgb * sFactor ) * 0.33;
  gl_FragColor = vec4 ( colour * atten * lambert , diffuse . a );
  gl_FragColor . rgb += ( diffuse . rgb * lightColour.rgb ) * 0.1;
 
	//gl_FragColor.rgb = (IN.normal + 1.0) * 0.5;
}

/*
# version 150 core

 uniform vec3  cameraPos ;
 uniform vec4  lightColour = vec4(1.0,1.0,1.0,1.0);
 uniform vec3  lightPos    = vec3(16.0,64.0,16.0);
 uniform float lightRadius = 5000;

 in Vertex {
   vec3 colour ;
   vec2 texCoord ;
   vec3 normal ;
   vec3 worldPos ;
 } IN ;

 out vec4 gl_FragColor ;


void main ( void ) {
 
 vec4 diffuse = vec4(IN.colour,1.0);
  vec3 incident = normalize ( lightPos - IN.worldPos );
  float lambert = max (0.0 , dot ( incident , IN.normal ));


  float dist = length ( lightPos - IN.worldPos );
  float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

  vec3 viewDir = normalize ( cameraPos - IN.worldPos );
  vec3 halfDir = normalize ( incident + viewDir );
  
  float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
  float sFactor = pow ( rFactor , 50.0 );


  vec3 colour = ( diffuse.rgb * lightColour.rgb );
  colour += ( lightColour.rgb * sFactor ) * 0.33;
  gl_FragColor = vec4 ( colour * atten * lambert , diffuse . a );
  gl_FragColor . rgb += ( diffuse . rgb * lightColour.rgb ) * 0.1;

}
*/