#include "renderer.h"



Renderer::Renderer(Window &parent) : OGLRenderer(parent) {

	camera = new Camera(0.0f,0.0f,Vector3(0.0f,0.0f,0.0f));

	loadResources();
	reloadShaders();


	init = true;

	noise64 = NoiseFunc::generate3DNoiseTexture(128,128,128);
	zTimer = 0.0f;
	camera->UpdateCamera(1.0f);

	
}

Renderer::~Renderer() {


}

void Renderer::reloadShaders() {
	

}

bool Renderer::loadResources() {
	testSquare = Mesh::GenerateQuad();
	testChunk = new TerrainChunk(Vector3(0.0f,0.0f,0.0f));

	texturedShader = new Shader(SHADERDIR"texturedVertex.glsl", SHADERDIR"3dtexTestFrag.glsl");
	
	if(!texturedShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	chunkShader = new Shader(SHADERDIR"chunkVertex.glsl", SHADERDIR"chunkFrag.glsl", SHADERDIR"chunkGeom.glsl"); //

	if(!chunkShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	return true;
}

void Renderer::UpdateScene(float msec){
	camera->UpdateCamera(msec);
	zTimer += msec * 0.0005;
	if(zTimer>1.0f) zTimer -=1.0f;
}

void Renderer::RenderScene(void){
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	glDisable(GL_CULL_FACE);
	SetCurrentShader(chunkShader);

	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Perspective(0.0f,10000.0f,16.0f / 9.0f, 45.0f);

	viewMatrix = camera->BuildViewMatrix();
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_3D,noise64);
	
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "threeDTex"),20);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "timer"),zTimer);

	UpdateShaderMatrices();
	
	testChunk->Draw();

	SwapBuffers();	
}

void Renderer::RenderNoiseCheck() {
	//Messy test shit
	SetCurrentShader(texturedShader);
	
	modelMatrix.ToIdentity();
	modelMatrix.Scale(Vector3(100.0f,100.0f,100.f));
	projMatrix = Matrix4::Perspective(0.0f,10000.0f,16.0f / 9.0f, 45.0f);
	viewMatrix = camera->BuildViewMatrix();
	
	
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_3D,noise64);
	
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "threeDTex"),20);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "timer"),zTimer);

	UpdateShaderMatrices();

	testSquare->Draw();



}