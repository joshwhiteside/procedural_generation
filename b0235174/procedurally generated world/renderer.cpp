#include "renderer.h"



Renderer::Renderer(Window &parent) : OGLRenderer(parent) {

	camera = new Camera(0.0f,0.0f,Vector3(0.0f,0.0f,0.0f));

	loadResources();
	reloadShaders();


	init = true;

	noise64 = NoiseFunc::generate3DNoiseTexture(128,128,128);
	zTimer = 0.0f;
	camera->UpdateCamera(1.0f);
	glEnable(GL_DEPTH_TEST);

}

Renderer::~Renderer() {


}

void Renderer::reloadShaders() {
	

}

bool Renderer::loadResources() {
	testSquare = Mesh::GenerateQuad();

	for(int x = 0; x < 9; x++){
		for(int z = 0; z < 9; z++){
			testChunk.push_back(new TerrainChunk(Vector3(32.0f*x,0.0f,32.0f*z)));
		}
	}
	texturedShader = new Shader(SHADERDIR"texturedVertex.glsl", SHADERDIR"3dtexTestFrag.glsl");
	
	if(!texturedShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	chunkShader = new Shader(SHADERDIR"chunkVertex.glsl", SHADERDIR"chunkFrag.glsl"); //

	if(!chunkShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	//testChunk->GenerateNewChunk(Vector3(0.0,0.0,0.0));


	return true;
}

void Renderer::UpdateScene(float msec){
	camera->UpdateCamera(msec);
	zTimer += msec * 0.0005;
	if(zTimer>1.0f) zTimer -=1.0f;
	/*
	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)){
		testChunk->GenerateNewChunk(Vector3(camera->GetPosition().x,0.0f,camera->GetPosition().z));
	}
	*/
}

void Renderer::RenderScene(void){
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	//glEnable(GL_CULL_FACE);
	SetCurrentShader(chunkShader);

	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Perspective(1.0f,DRAW_DISTANCE,
		(float)width / (float)height, 45.0f);

	viewMatrix = camera->BuildViewMatrix();
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_3D,noise64);
	
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "threeDTex"),20);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "timer"),zTimer);


	for(vector<TerrainChunk*>::iterator i = testChunk.begin(); i != testChunk.end(); i++){
		modelMatrix.ToIdentity();
		modelMatrix = Matrix4::Translation((*i)->getWorldPos());

		UpdateShaderMatrices();

		(*i)->Draw();
	}
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


void	Renderer::updateTerrain(){
	
		int x = (int)camera->GetPosition().x;
		int y = (int)camera->GetPosition().z;
		x = x / TERRAIN_GRID_WIDTH;
		y = y / TERRAIN_GRID_WIDTH;
		if(camera->GetPosition().x<0) x--;
		if(camera->GetPosition().z<0) y--;



		if(x!=cameraGridX || y!=cameraGridZ) {
			RequestNewTerrain(x,y);
			if(x!=cameraGridX)		{
				int step;
				if(x<cameraGridX) step=-1;
				if(x>cameraGridX) step=1;
				RequestNewTerrain(x,y-1);
				RequestNewTerrain(x,y+1);
				//RequestNewTerrain(x+(2*step),y);
				RequestNewTerrain(x+step,y);
				//RequestNewTerrain(x+step,y-1);
				//RequestNewTerrain(x+step,y+1);
			}	
			if(y!=cameraGridZ){
				int step;
				if(y<cameraGridZ) step=-1;
				if(y>cameraGridZ) step=1;

				RequestNewTerrain(x+1,y);
				RequestNewTerrain(x-1,y);
				//RequestNewTerrain(x,y+(2*step));
				RequestNewTerrain(x,y+step);
				//RequestNewTerrain(x-1,y+step);
				//RequestNewTerrain(x+1,y+step);
			}
		}
		cameraGridX = x;
		cameraGridZ = y;
	}


void	Renderer::RequestNewTerrain(int x, int y) {

	
	for(vector<TerrainChunk*>::iterator i = testChunk.begin(); i != testChunk.end(); i++){
			
	}


	/*
	for(int i = 0; i < NO_CHUNKS; i++) {
		if (terrainChunk[i]->isSameGridPos(x,y))
			return;
	}

	MapNode* n = findFurthestChunk();
	n->reassign(network, x,y);
	*/
}

TerrainChunk*	Renderer::findFurthestChunk() {
	TerrainChunk* t;
	/*
	MapNode* retChunk = terrainChunk[0];
	Vector3 cp		  = camera->getWorldPosition();

	float highestDistance = 0.0f;

	for(int n = 0; n<NO_CHUNKS; n++) {
		float currentDistance = Vector3(cp - terrainChunk[n]->getWorldPos()).LengthSq();
		if(currentDistance>highestDistance){
			retChunk = terrainChunk[n];
			highestDistance = currentDistance;
		}
	}

	return retChunk;
	*/
	
	return t;
}
