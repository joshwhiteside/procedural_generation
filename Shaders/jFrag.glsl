#version 150 core


in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

uniform vec2 cycle;
uniform sampler2D diffuseTex;


int max_iterations = 20;
out vec4 gl_FragColor;


void main(void) {
	vec2 tempc= cycle;
	
	int iterations = 0;
	vec2 z = IN.texCoord.xy;
	const float threshold_squared = 4.0;
	
	vec4 outputcolour;
	while(iterations < max_iterations && dot(z,z) < threshold_squared)
	{
		vec2 z_squared;
		z_squared.x = (z.x * z.x) - (z.y * z.y);
		z_squared.y = 2.0 * z.x * z.y;		
		z = sin(z_squared) + sin(tempc);
		iterations++;
	}
	if(iterations == max_iterations) {
		outputcolour = vec4(0.25,0.75, 1.0, 1.0);
		//outputcolour = vec4(0.0, 0.0, 0.0, 1.0);
		

	} else {
	
		float f = iterations;
		f /= max_iterations;
		vec2 temp = vec2(0.5,f);
		
		outputcolour = mix(vec4(0.5,0.5,1.0,1.0),vec4(1.0,0.5,0.9,1.0), f);
		//outputcolour = vec4(1.0,0.0,0.0,1.0);
	}
	

	gl_FragColor = outputcolour;
	
	
}