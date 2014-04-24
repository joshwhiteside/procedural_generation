#include "renderer.h"


Renderer::Renderer(Window &parent) : OGLRenderer(parent) {

	quad->GenerateQuad();
	

	quad->SetcolorMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));



	jShader = new Shader(SHADERDIR"jvert.glsl",
						 SHADERDIR"jfrag.glsl");

	if(!jShader->LinkProgram() || !quad->GetcolorMap() ) {
		return;
	}

	SetCurrentShader(jShader);


	init = true;
}

Renderer::~Renderer() {
	delete quad;
	delete jShader;

	currentShader = NULL;
}

void Renderer::UpdateScene(float msec) {
	quad->Update();
}

void Renderer::RenderScene(){ 
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(currentShader->GetProgram());
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
								     "c") , quad->getCycle());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"colorMap"), quad->GetcolorMap());

	quad->Draw();

	glUseProgram(0);	
}
