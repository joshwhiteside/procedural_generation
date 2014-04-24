#version 400

layout (vertices = 3) out;

in vec3 controlpoint_wor[];

out vec3 evaluationpoint_wor[];

uniform float tessLevelInner = 4.0;
uniform float tessLevelOuter = 4.0;

void main() {
	evaluationpoint_wor[gl_InvocationID] = controlpoint_wor[gl_InvocationID];
	
	gl_TessLevelInner[0] = tessLevelInner;
	
	gl_TessLevelOuter[0] = tessLevelOuter; 	
	gl_TessLevelOuter[1] = tessLevelOuter; 	
	gl_TessLevelOuter[2] = tessLevelOuter; 	



}