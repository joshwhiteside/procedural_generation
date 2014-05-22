#include "renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
	TerrainChunk::initNoiseLayers(); //Saves loading time later..

	camera = new Camera(0.0f,0.0f,Vector3(0.0f,0.0f,0.0f));

	loadResources();
	reloadShaders();

	init = true;

	noise64  = NoiseFunc::generate3DNoiseTexture(64,64,64);
	cloudTex = NoiseFunc::generate3DNoiseTexture(1024,1024,8); 

	zTimer = 0.0f;
	fTimer = 0.0f;
	cTimer = 0.0f;

	camera->UpdateCamera(1.0f);
	glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {
	delete light;
	delete skyquad;
	delete testSquare;


}

void Renderer::reloadShaders() {
	

}

bool Renderer::loadResources() {
	testSquare  = Mesh::GenerateQuad();
	skyquad		= Mesh::GenerateQuad();

	for(int x = 0; x < CHUNK_GRID_SIZE; x++){
		for(int z = 0; z < CHUNK_GRID_SIZE; z++){
			testChunk.push_back(new TerrainChunk(x,z));
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

	
	skyShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl"); //

	if(!skyShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	cloudShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"cloudFragment.glsl"); //

	if(!cloudShader->LinkProgram()){
		cout << "fuck" << endl;
	}

	light = new Light();
	light->SetColour(Vector4(1.0,1.0,1.0,1.0));
	light->SetRadius(LIGHT_RADIUS);

	return true;
}

void Renderer::UpdateScene(float msec){
	camera->UpdateCamera(msec);
	zTimer += msec * 0.0005f;
	fTimer += msec * 0.1f;
	cTimer += msec * 0.00001f;
	//if(fTimer > F_TIMER_MAX ) fTimer -= F_TIMER_MAX;
	if(zTimer>1.0f) zTimer -=1.0f;
	updateTerrain();

	light->SetPosition(camera->GetPosition());

	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)){
		NoiseFunc::perlinNoise3D(camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z,9,0.75f);
	}	
}

void Renderer::RenderScene(void){
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_M)){
		RenderNoiseCheck();
	} else {


		RenderSky();

		SetCurrentShader(chunkShader);

		SetShaderLight(*light);

		modelMatrix.ToIdentity();
		projMatrix = Matrix4::Perspective(1.0f,DRAW_DISTANCE,
			(float)width / (float)height, 45.0f);

		viewMatrix = camera->BuildViewMatrix();
		glActiveTexture(GL_TEXTURE20);
		glBindTexture(GL_TEXTURE_3D,noise64);
	
	
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "noiseTex"),20);
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "timer"),fTimer);
		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
			"cameraPos"), 1, (float*)&camera->GetPosition());
	

		for(vector<TerrainChunk*>::iterator i = testChunk.begin(); i != testChunk.end(); i++){
			modelMatrix.ToIdentity();
			modelMatrix = Matrix4::Translation((*i)->getWorldPos() * TERRAIN_DRAW_SIZE) * Matrix4::Scale(Vector3(TERRAIN_DRAW_SIZE,TERRAIN_DRAW_SIZE,TERRAIN_DRAW_SIZE));
			//modelMatrix = Matrix4::Translation((*i)->getWorldPos() * TERRAIN_DRAW_SIZE);


			UpdateShaderMatrices();

			(*i)->Draw();
		}
	}
	SwapBuffers();	
}

void Renderer::RenderSky() {
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(skyShader);
	
	projMatrix = Matrix4::Perspective(1.0f,DRAW_DISTANCE,
			(float)width / (float)height, 45.0f);
	
	//projMatrix = Matrix4::Perspective(1.0f,10000.0f,(float)width/height,45.0f);
	viewMatrix = camera->BuildViewMatrix();
	modelMatrix.ToIdentity();

	UpdateShaderMatrices();
	
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_3D,noise64);
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "noiseTex"),20);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"),fTimer);

	skyquad->Draw();

	glUseProgram(0);
	
	SetCurrentShader(cloudShader);

	UpdateShaderMatrices();

	
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_3D,cloudTex);
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cloudTex"),20);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"),cTimer);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	skyquad->Draw();
	glDisable(GL_BLEND);
	glUseProgram(0);
	
	glDepthMask(GL_TRUE);
	
	glEnable(GL_CULL_FACE);
}

void Renderer::RenderNoiseCheck() {
	//Messy test shit
	SetCurrentShader(texturedShader);
	
	modelMatrix.ToIdentity();
	//modelMatrix.Scale(Vector3(100.0f,100.0f,100.f));
	projMatrix = Matrix4::Orthographic(-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f);
	viewMatrix.ToIdentity();
	
	
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
		x = x / (TERRAIN_GRID_WIDTH * TERRAIN_DRAW_SIZE);
		y = y / (TERRAIN_GRID_WIDTH * TERRAIN_DRAW_SIZE);
		if(camera->GetPosition().x<0) x--;
		if(camera->GetPosition().z<0) y--;


		
		if(x!=cameraGridX || y!=cameraGridZ) {
			x-= CHUNK_LOAD_RADIUS / 2;
			y-= CHUNK_LOAD_RADIUS / 2;
			for(int m = 0; m < CHUNK_LOAD_RADIUS; m++){
				for(int n = 0; n < CHUNK_LOAD_RADIUS; n++){
					RequestNewTerrain(m+x,n+y);
				}
			}
		}
			
		cameraGridX = x;
		cameraGridZ = y;
}	 

void	Renderer::RequestNewTerrain(int x, int y) {	
	for(vector<TerrainChunk*>::iterator i = testChunk.begin(); i != testChunk.end(); i++){
		if ((*i)->isSameGridPos(x,y))
			return;
	}

	TerrainChunk* n = findFurthestChunk();
	n->reassign(x,y);
	
}

TerrainChunk*	Renderer::findFurthestChunk() {	
	TerrainChunk* retChunk = *testChunk.begin();
	Vector3 cp		  = camera->GetPosition();

	float highestDistance = 0.0f;

	for(vector<TerrainChunk*>::iterator i = testChunk.begin(); i != testChunk.end(); i++){
		float currentDistance = Vector3(cp - (*i)->getWorldPos()).LengthSq();
		if(currentDistance>highestDistance){
			retChunk = (*i);
			highestDistance = currentDistance;
		}
	}

	return retChunk;
}