#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float time;

 in vec3 position ;
 in vec4 colour ;
 in vec3 normal ;
 in vec3 tangent;
 in vec2 texCoord ;

 uniform sampler2D juliaTex;

 
 
 out Vertex {
 vec4 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 tangent;
 vec3 binormal;
 vec3 worldPos ;
} OUT ;


void main(void)	{
	float timeofday = sin(time * 3.142);
	//float watertime = sin(time * 3.142 * 64);
	vec2 juliaInfluence = vec2(position.x / (512 * 16) , position.z / (512 * 16));
	float fractalJump = texture2D(juliaTex, juliaInfluence).g;
	OUT.colour = texture2D(juliaTex, juliaInfluence);

	
	float y_trans = (sin((time * 3.142 * 360)+(position.x/180*3.142)) + sin((position.z/180*3.142)+ (time * 3.142 * 360))) * 6;   
	if(timeofday< 0.5) {
		y_trans += texture2D(juliaTex, juliaInfluence).g * 40;
		y_trans -= texture2D(juliaTex, juliaInfluence).r * 40;
	}
	
	mat4 trans;
	trans[0] = vec4(1,0,0,0);
	trans[1] = vec4(0,1,0,0);
	trans[2] = vec4(0,0,1,0);
	trans[3] = vec4(0,y_trans,0,1);
	
	trans= modelMatrix * trans;


	OUT.texCoord = ( textureMatrix * vec4 ( texCoord , 0.0 , 1.0)). xy ;
	
	
	
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));


	OUT.normal = normalize ( normalMatrix * normalize ( normal ));
	OUT.tangent= normalize ( normalMatrix * normalize( tangent ));
	OUT.binormal=normalize(normalMatrix * 
			 normalize(cross(normal,tangent)));
			 
			 
	OUT.worldPos = ( trans * vec4 ( position ,1)).xyz ;

	gl_Position = ( projMatrix * viewMatrix * trans ) * vec4 ( position , 1.0);
	
	//gl_Position.y +=y_trans;
}