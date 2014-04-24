#include "renderer.h"



Renderer::Renderer(Window &parent) : OGLRenderer(parent) {

	camera = new Camera(0.0f,0.0f,Vector3(0.0f,0.0f,0.0f));

	loadResources();
	reloadShaders();


	init = true;

	noise64 = NoiseFunc::generate3DNoiseTexture(128,128,128);
	

}

Renderer::~Renderer() {


}

void Renderer::reloadShaders() {
	

}

bool Renderer::loadResources() {
	testSquare = Mesh::GenerateQuad();

	texturedShader = new Shader(SHADERDIR"texturedVertex.glsl", SHADERDIR"3dtexTestFrag.glsl");
	if(!texturedShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	return true;
}

void Renderer::UpdateScene(float msec){
	camera->UpdateCamera(msec);
}

void Renderer::RenderScene(void){
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	//Messy test shit
	SetCurrentShader(texturedShader);
	
	modelMatrix.ToIdentity();
	modelMatrix.Scale(Vector3(100.0f,100.0f,100.f));
	projMatrix.Perspective(0.0f,10000.0f,16.0f / 9.0f, 45.0f);
	viewMatrix = camera->BuildViewMatrix();
	
	
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_3D,noise64);
	
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "threeDTex"),20);

	UpdateShaderMatrices();

	testSquare->Draw();


	SwapBuffers();	
}