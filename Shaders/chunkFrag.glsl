
#version 150 core

uniform sampler3D threeDTex;
uniform float timer;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 position;
	vec3 worldPos;
} IN;

out vec4 gl_FragColor;


void main(void)	{

	
	
	vec4 col1 = vec4(1.0,0.0,1.0,1.0);
	vec4 col2 = vec4(0.0,1.0,1.0,1.0);
	//vec4 tempcolour = texture(threeDTex, vec3(IN.worldPos* 0.0001).rrra;
	
	float mixVal =  IN.worldPos.y * 0.05;
	vec4 tempcolour = vec4(0.0);
	tempcolour = IN.colour;
	//tempcolour.rgb = vec3(IN.worldPos.x * 0.05,IN.worldPos.y * 0.05,IN.worldPos.z * 0.05);//mix(col1,col2,mixVal);
	//tempcolour = vec4(IN.texCoord,0.0,1.0);
	tempcolour.a = 1.0;
	gl_FragColor = tempcolour;
	
}




//Lights are not ready...
/*
# version 150 core

 uniform sampler2D diffuseTex ;

 uniform vec3  cameraPos ;
 uniform vec4  lightColour = vec4(1.0,1.0,1.0,1.0);
 uniform vec3  lightPos    = vec3(16.0,16.0,16.0);
 uniform float lightRadius = 500;

 in Vertex {
   vec3 colour ;
   vec2 texCoord ;
   vec3 normal ;
   vec3 worldPos ;
 } IN ;

 out vec4 gl_FragColor ;


void main ( void ) {
 
  //vec4 diffuse  = vec4(IN.worldPos.x * 0.05,IN.worldPos.y * 0.05,IN.worldPos.z * 0.05,1.0);
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

  gl_FragColor = vec4(IN.normal,1.0 );


//gl_FragColor.rgb = colour * atten  ;  

//gl_FragColor = vec4( atten );
//gl_FragColor.a = 1.0;

}
*/