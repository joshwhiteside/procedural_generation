#include "renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera		= new Camera(0.0f,135.0f,Vector3(0,500,0));
	camera->SetPosition(Vector3(0,450,0));
	camera->SetYaw(240);
	quad		= Mesh::GenerateQuad();

	heightMap	= new HeightMap(TEXTUREDIR"terrain.raw");
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG",
	SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS));

	sobelTest = SOIL_load_OGL_texture(TEXTUREDIR"Bikesgray.jpg",	SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS);


	sceneShader		= new Shader(SHADERDIR"TexturedVertex.glsl",
								 SHADERDIR"TexturedFragment.glsl");
	processShader	= new Shader(SHADERDIR"TexturedVertex.glsl",
								 SHADERDIR"sobelfragment.glsl");
	depthShader		= new Shader(SHADERDIR"TexturedVertex.glsl",
								 SHADERDIR"DrawDepthFrag.glsl");

	if(!processShader->LinkProgram() || !sceneShader->LinkProgram() || !depthShader->LinkProgram() || 
		!heightMap->GetTexture() || !sobelTest) {
			return;
	}

	SetTextureRepeating(heightMap->GetTexture(),true);

	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
				 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for(int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1,&bufferFBO); //We'll render the scene into this
	glGenFramebuffers(1,&processFBO);//And do post processing in this ;)

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,							GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,							GL_TEXTURE_2D, bufferColourTex[0], 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
	GL_FRAMEBUFFER_COMPLETE || !sceneDepthTex || !sceneColourTex[0])	{
		return;		
	}
	postProcessOn = true;

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glEnable(GL_DEPTH_TEST);
	init = true;
	
}

Renderer::~Renderer(void)	{
	delete sceneShader;
	delete processShader;
	currentShader = NULL;

	delete heightMap;
	delete quad;
	delete camera;

	glDeleteTextures(2,bufferColourTex);
	glDeleteTextures(1,&bufferDepthTex);
	glDeleteFramebuffers(1,&bufferFBO);
	glDeleteFramebuffers(1,&processFBO);
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();

	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
	{
		postProcessOn = !postProcessOn;
	}
}

void Renderer::RenderScene()	{
	
	if(postProcessOn)
	{
		DrawScene(false);
	
		
		PresentScene(sobelTest, true);

		DrawPostProcess();
		PresentScene(bufferColourTex[0]);
	}
	else
	{
		DrawScene(false);
		DrawDepthBuffer();
	}


	SwapBuffers();
}

void Renderer::DrawScene(bool backBuffer) {

	if(backBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
	}



	SetCurrentShader(sceneShader);
	projMatrix		= Matrix4::Perspective(1.0f,10000.0f,
							   (float) width / (float) height, 45.0f);
	
	viewMatrix = camera->BuildViewMatrix();

	UpdateShaderMatrices();

	heightMap->Draw();
	
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0 );
}


void Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT ) ;

	SetCurrentShader(processShader);

	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),		"pixelSize"), 1.0f / width, 1.0f /height);


	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, sobelTest);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "originalImage"), 5);
	glActiveTexture(GL_TEXTURE0);



	for(int i =0 ; i < 1; ++i)	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);
		
		quad->SetTexture(bufferColourTex[0]);
		quad->Draw();

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);


		quad->SetTexture(bufferColourTex[1]);
		quad->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0 );
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}


void Renderer::PresentScene(GLuint textureToDisplay, bool backBuffer)	{
	if(backBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}
	SetCurrentShader(sceneShader);
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	//quad->SetTexture(bufferColourTex[0]);
	quad->SetTexture(textureToDisplay);
	quad->Draw();
	glUseProgram(0);
}

void Renderer::DrawDepthBuffer()	{
	SetCurrentShader(depthShader);
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	//quad->SetTexture(bufferColourTex[0]);
	quad->SetTexture(bufferDepthTex);
	quad->Draw();
	glUseProgram(0);
}