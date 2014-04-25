#include "renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	heightMap		= new HeightMap(TEXTUREDIR"terrain.raw");
	camera			= new Camera();

	mountainShader	= new Shader(SHADERDIR"TexturedVertex.glsl",
								 SHADERDIR"TexturedFragment.glsl");
	waterShader		= new Shader(SHADERDIR"WaterVertex.glsl",
								 SHADERDIR"WaterFragment.glsl");

	if(!waterShader->LinkProgram()) {
		return;
	}


	if(!mountainShader->LinkProgram()) {
		return;
	}
	
	heightMap->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR"swirl.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture2(SOIL_load_OGL_texture(
		TEXTUREDIR"fractal.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		
	if(!heightMap->GetTexture()) {
		return;
	}
	if(!heightMap->GetTexture2()) {
		return;
	}

 	SetTextureRepeating(heightMap->GetTexture(),true);
	SetTextureRepeating(heightMap->GetTexture2(),true);
	projMatrix = Matrix4::Perspective(1.0f,10000.0f,
									 (float)width/(float)height,45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	cycle = 0;

	init = true;
}


Renderer::~Renderer(void)	{
	delete heightMap;
	delete camera;
	
	
}

void Renderer::UpdateScene(float msec)	{
	camera->UpdateCamera(msec);
	viewMatrix	= camera->BuildViewMatrix();
}

void Renderer::RenderScene() {
	cycle +=0.1f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetCurrentShader(mountainShader);
	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
				"diffuseTex"), 0);

	heightMap->Draw();
	glUseProgram(0);

	SetCurrentShader(waterShader);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
				"secondTex"), 1);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
				"time") , cycle);
	heightMap->Draw();
	glDisable(GL_BLEND);
	glUseProgram(0);

	SwapBuffers();

}