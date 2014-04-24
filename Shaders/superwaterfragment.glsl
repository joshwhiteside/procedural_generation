#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform samplerCube cubeTex;
uniform samplerCube cubeTex2;



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

	vec4 modcolour = IN.colour;
	modcolour.g *=0.5;
	float timeofday = sin(time * 3.142);
	
	vec4 ambient = mix( vec4(0.0, 0.0, 0.0, 1.0), vec4(0.3,0.3,0.3,1.0), timeofday);

	//vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	
	vec4 diffuse  = texture ( diffuseTex , IN.texCoord );
	
	mat3 TBN		= mat3(IN.tangent, IN.binormal, IN.normal);
	//vec3 normal		= normalize(TBN *(((texture2D(bumpTex, IN.texCoord). rgb)* 2.0)-1.0));
	vec3 normal = IN.normal;
					  
					  
	vec3 incident = normalize (lightPos - IN.worldPos);
	
	vec3 incident2 = normalize (IN.worldPos - cameraPos);
	
	float lambert = max (0.0 , dot (incident , normal));
	
	vec4 ref1 = texture(cubeTex2, reflect(incident2, normalize(normal)));
	vec4 ref2 = texture(cubeTex, reflect(incident2, normalize(normal)));
	vec4	reflection  = mix(
								ref1,
								ref2,
								timeofday);
						  
	diffuse = mix(reflection, diffuse, 0.9);
	
	ambient *= diffuse;
	if(timeofday < 0.6)
	ambient	= mix(modcolour, ambient, timeofday);
	ambient = mix(reflection, ambient, 0.5);
	
	

	float dist = length (lightPos - IN.worldPos);
	float atten = 1.0 - clamp (dist / lightRadius , 0.0 , 1.0);

	vec3 viewDir = normalize (cameraPos - IN.worldPos);
	vec3 halfDir = normalize (incident + viewDir);
  
	float rFactor = max (0.0 , dot (halfDir,normal));
	float sFactor = pow (rFactor , 50.0);


	vec3 colour = (diffuse.rgb * lightColour.rgb);
  
 
	colour += (lightColour.rgb * sFactor ) * 0.33;

	gl_FragColor[0] = ambient;
	
	gl_FragColor[0]	+= vec4 ( colour * atten * lambert , diffuse.a );

	//gl_FragColor[0].rgb = IN.normal;
	//gl_FragColor[0] = colour;
	
	//gl_FragColor[1] = vec4(normal.xyz * 0.5 + 0.5 , 1.0);
	
	//gl_FragColor[0] = vec4(normal.xyz,1.0);
	//gl_FragColor[0].rgb = texture2D(bumpTex, IN.texCoord). rgb;
	//gl_FragColor[0]	= IN.texCoord;

}