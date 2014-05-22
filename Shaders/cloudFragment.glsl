#version 400 core

uniform vec3		cameraPos;
uniform float		time;
uniform float		dayRatio;//
uniform vec3 lightPos;
uniform sampler3D cloudTex;

in Vertex	{
	vec3 normal;
} IN;

out vec4 gl_FragColor;

uniform vec4 auroraColour2= vec4(0.0,0.0,1.0,1.0);
uniform vec4 auroraColour = vec4(0.0,1.0,0.0,1.0);

int max_iterations = 20;

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

void main(void){
	vec3 cloudPos = vec3(normalize(IN.normal+vec3(time)));
	//cloudPos.x += time;
	cloudPos.xy = vec2(cloudPos.x , cloudPos.y * cloudPos.z);
	cloudPos *= 0.0125;
	//cloudPos  += time;
	cloudPos.z = time * 2;
	
	float cloudVal =  0.25;
	cloudVal += texture(cloudTex, cloudPos);
	//cloudVal *= cloudVal;
	
	//gl_FragColor = vec4(1.0,1.0,1.0,cloudVal );
	
	float transparency = (cloudVal * cloudVal) - 0.5;
	
	gl_FragColor = vec4(cloudVal ,cloudVal ,cloudVal ,transparency);
	
	
}