#version 410


layout(triangles,equal_spacing,ccw) in;
in vec3 evaluationpoint_wor[];



void main() {
	vec3 p0 = gl_TessCoord.x * evaluationpoint_wor[0];
	vec3 p1 = gl_TessCoord.y * evaluationpoint_wor[1];
	vec3 p2 = gl_TessCoord.z * evaluationpoint_wor[2];
	
	vec3 pos = p0 + p1 + p2;
	
	gl_Position = vec4(pos,1.0);



}