
#version 150 core


 uniform vec3  cameraPos ;
 uniform vec4  lightColour = vec4(1.0,1.0,1.0,1.0);
 uniform vec3  lightPos    = vec3(16.0,64.0,16.0);
 uniform float lightRadius = 5000;


uniform sampler3D threeDTex;
uniform float timer;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
	vec3 worldPos;
	vec3 normal;
} IN;

out vec4 gl_FragColor;


void main(void)	{

	vec4 diffuse = vec4(IN.colour);
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