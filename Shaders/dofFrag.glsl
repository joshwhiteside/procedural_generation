#version 150 core

uniform sampler2D diffuseTex; //Used in this case for the depth buffer
uniform sampler2D secondTex; //Used in this case for the blurred image
uniform vec2 pixelSize;


uniform sampler2D bumpTex; //Used in this case for the original image.

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
} IN;

out vec4 gl_FragColor[2];

float linearizeDepth(vec2 uv) {
	float n = 1.0;
	float f = 10000.0;
	float z = texture(diffuseTex, uv).r;
	return (2.0*n) /(f+n -z * (f-n));
}


void main(void) {
	float ratio = linearizeDepth(IN.texCoord);
	vec4 blur = texture(secondTex,IN.texCoord);
	vec4 image= texture(bumpTex,IN.texCoord);
	
	gl_FragColor[0] = mix(image,blur,ratio);
	
}