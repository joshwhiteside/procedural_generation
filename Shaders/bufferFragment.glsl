#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

uniform vec3  cameraPos ;
uniform vec4  lightColour ;
uniform vec3  lightPos ;
uniform float lightRadius ;

uniform float time;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 gl_FragColor[2];

void main(void) {

	float ratio = time;
	ratio -= 3600;
	if (ratio < 0) ratio = -time;
	ratio /= 3600;
	
	//vec4 ambient = mix( vec4(0.2, 0.2, 0.3, 1.0), vec4(0.3,0.3,0.3,1.0), ratio);

	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	
	vec4 diffuse  = texture ( diffuseTex , IN.texCoord );
	
	ambient *= diffuse;
	
	mat3 TBN		= mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal		= normalize(TBN *(((texture2D(bumpTex, IN.texCoord). rgb)* 2.0)-1.0));
					  
					  
	vec3 incident = normalize (lightPos - IN.worldPos);
	float lambert = max (0.0 , dot (incident , normal));


	float dist = length (lightPos - IN.worldPos);
	float atten = 1.0 - clamp (dist / lightRadius , 0.0 , 1.0);

	vec3 viewDir = normalize (cameraPos - IN.worldPos);
	vec3 halfDir = normalize (incident + viewDir);
  
	float rFactor = max (0.0 , dot (halfDir,normal));
	float sFactor = pow (rFactor , 50.0);


	vec3 colour = (diffuse.rgb * lightColour.rgb);
  
 
	colour += (lightColour.rgb * sFactor ) * 0.33;
 // gl_FragColor 
	gl_FragColor[0] = ambient;
	
	gl_FragColor[0]	+= vec4 ( colour * atten * lambert , diffuse.a );
	//gl_FragColor[0] = vec4(vec3(lambert), 1.0);
	//gl_FragColor[0] = vec4(vec3(atten), 1.0);
	//gl_FragColor[0] = vec4(IN.worldPos/ 2048.0, 1.0);
	//gl_FragColor[0] = vec4(lightPos/2048, 1.0);
	//gl_FragColor[0] = vec4(IN.normal, 1.0);
	//gl_FragColor[0] = vec4(IN.tangent, 1.0);
	//gl_FragColor[0] = vec4(IN.binormal, 1.0);
	
	//gl_FragColor[0] = vec4(colour.r,colour.g,colour.b, 1.0);
	//gl_FragColor[0] = vec4(colour * atten * lambert, 1.0);
	//gl_FragColor[1] = vec4(normal.xyz * 0.5 + 0.5 , 1.0);

}