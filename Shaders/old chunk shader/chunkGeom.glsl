

#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


uniform sampler3D threeDTex;

in Vertex {
	vec3 worldPos;
	vec4 colour;
	float particleSize;
} IN[];

out Vertex {
	vec4 colour;
	vec3 worldPos;
	vec2 texCoord;
} OUT;
 
void main() {	

	float particleSize;
	for(int i = 0; i < gl_in.length(); ++i) {
		OUT.worldPos = IN[i].worldPos;
		OUT.colour = IN[i].colour;
		float particleSize = 0.1;
		
		if(IN[i].worldPos.y < 4 || IN[i].particleSize > 0.85 || IN[i].particleSize < 0.15){

		//top right
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x += particleSize;
		gl_Position.y += particleSize;
		OUT.texCoord = vec2(1,0);

		//When we've created a vertex, we call the glsl in-built
		//function EmitVertex, which sends our vertex off for
		//rasterisation.
		EmitVertex();

		//Then we do the other vertices of the quad...
		//Top Left
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x -= particleSize;
		gl_Position.y += particleSize;
		OUT.texCoord = vec2(0,0);
		OUT.worldPos = IN[i].worldPos;
		EmitVertex();

		//bottom right
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x += particleSize;
		gl_Position.y -= particleSize;
		OUT.texCoord = vec2(1,1);
		OUT.worldPos = IN[i].worldPos;
		EmitVertex();

		//bottom Left
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x -= particleSize;
		gl_Position.y -= particleSize;
		OUT.texCoord = vec2(0,1);
		OUT.worldPos = IN[i].worldPos;
		EmitVertex();

		
		EndPrimitive();
		}
	}
}