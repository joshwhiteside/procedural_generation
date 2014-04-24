#include "renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	rotation = 0.0f;
	time = 0.0f;
	

	clock = new DayClock(18);
	moonPos = Vector3(0.0f,0.0f,0.0f);
	birdPos = Vector3(0.0f,0.0f,0.0f);
	speed = 1.0f;
	juliatime = Vector2(0.0f,0.0f);


	camera	 = new Camera(0.0f,0.0f,
		Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500, RAW_WIDTH*HEIGHTMAP_X));
	

	pointLights = new Light[LIGHTNUM*LIGHTNUM];
	for(int x = 0; x<LIGHTNUM*LIGHTNUM; ++x) {
		
		Light &l = pointLights[x];
		float xPos = RAW_WIDTH*HEIGHTMAP_X / 2 + (1536 * sin((3.142f/32)*x));
		float zPos = RAW_WIDTH*HEIGHTMAP_Z / 2 + (1536 * cos((3.142f/32)*x));
		l.SetPosition(Vector3(xPos, 256.0f, zPos));

		float r = 0.5f + (float)(rand()%129) / 128.0f;
		float g = 0.5f + (float)(rand()%129) / 128.0f;
		float b = 0.5f + (float)(rand()%129) / 128.0f;	
		l.SetColour(Vector4(r,g,b,1.0f));

		float radius = (512);
		l.SetRadius(radius);
		
	}
	

	if(!GenerateBuffers())
		return;

	if(!LoadShaders())
		return;

	if(!LoadResources())
		return;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	sunLight = new Light(Vector3(2048,500,2048),Vector4(1.0f,1.0f,1.0f,1.0f),DAY_RADIUS);

	init = true;
	
}

Renderer::~Renderer(void) {
	delete		sceneShader;
	delete		combineShader;
	delete		pointlightShader;

	delete		heightMap;
	delete		camera;
	delete		sphere;
	delete		quad;
	delete[]	pointLights;
	delete		sun;
	delete		groudon;
	delete		waterMap;

	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteTextures(1, &juliaTex);
	glDeleteTextures(1, &pipColourTex);

	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &pointLightFBO);
	glDeleteFramebuffers(1, &pipFBO);
	glDeleteFramebuffers(1, &juliaFBO);
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	rotation = msec * 0.01f * clock->getSpeed();
	juliatime.x+=(0.01f*clock->getSpeed());
	juliatime.y+=(0.01f*clock->getSpeed());

	clock->Update(msec);
	/*
	cout << "Hour " << clock->GetHour() << endl;
	cout << "Minute " << clock->GetMinute() << endl;
	cout << "Second " << clock->GetSecond() << endl;
	cout << "ZeroOne " << clock->GetTime01() << endl;
	*/

	float x = 2048 + 2048 * (cos(clock->GetTime01()*3.142*12));
	float y = 2048 + 2048 * (sin(clock->GetTime01()*3.142*12));

	//sunLight->SetPosition(Vector3(x,y,x));
	sunLight->SetPosition(Vector3(2048.0f + cos(clock->GetTime01() * 3.142f) * 8184.0f,sin(clock->GetTime01() * 3.142) * 2048.0f,2048.0f + cos(clock->GetTime01() * 3.142f )* 8184.0f));
	float tempmoon = cos(clock->GetTime01()*3.142);
	if (tempmoon < 0.0f) tempmoon*=-1.0f;
	moonPos=Vector3(2048.0f + cos(clock->GetTime01() * 3.142f) * 8184.0f,(tempmoon) * 2048.0f,2048.0f + cos(clock->GetTime01() * 3.142f )* 8184.0f);


	if(clock->GetTime01() * 3.142f > 0.5f) 
		be->setalive(true); 
	else
		be->setalive(false);
	
	birdPos=Vector3(x,256.0f,y);



	if(Window::GetKeyboard()->KeyDown(KEYBOARD_F10)){
		ReloadShaders();
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_UP)){
		clock->HigherSpeed();
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)){
		clock->LowerSpeed();
	}

	be->Update(msec);

}

void Renderer::RenderScene() {
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawJuliaSet();
	
	FillBuffers();
	DrawPointLights();
	CombineBuffers();
	
	ShowScene();

	RenderText();

	SwapBuffers();
}

void Renderer::RenderText() {
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(textShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"diffuseTex"),0);
	textureMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1.0f,1.0f,(float)width, 0.0f,(float)height, 0.0f);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	//projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);

	DrawText(std::to_string(static_cast<long long>(fps)), Vector3(0,0,0),24.0f);


	projMatrix = Matrix4::Perspective(1.0f,10000.0f,(float) width / (float) height, 45.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size)	{

	TextMesh* mesh = new TextMesh(text,*basicFont);

	modelMatrix = Matrix4::Translation(Vector3(position.x,height-position.y, position.z)) * Matrix4::Scale(Vector3(size,size,1));
	
	//projMatrix = Matrix4::Orthographic(-1.0f,1.0f,(float)width, 0.0f,(float)height, 0.0f);
	
	UpdateShaderMatrices();
	mesh->Draw();


	delete mesh;
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	
	SetCurrentShader(skyboxShader);
	projMatrix = Matrix4::Perspective(1.0f,10000.0f,(float)width/height,45.0f);
	viewMatrix = camera->BuildViewMatrix();
	modelMatrix.ToIdentity();


	
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), clock->GetTime01());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"),9);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_CUBE_MAP,  cubeMap);
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex2"),10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP,  cubeMap2);

	UpdateShaderMatrices();


	glDisable(GL_CULL_FACE);

	
	quad->Draw();

	
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

}

void Renderer::ShowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	SetCurrentShader(outputShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"colourTex"),5);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"depthTex"),6);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"normalTex"),13);
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),"pixelSize"),1.0f/width,1.0f/height);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D,pipColourTex);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D,bufferDepthTex);
	
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D,bufferNormalTex);

	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	UpdateShaderMatrices();

	quad->Draw();
}

void Renderer::DrawJuliaSet() {
	glBindFramebuffer(GL_FRAMEBUFFER,juliaFBO);
	//glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(juliaShader);

	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	UpdateShaderMatrices();

	glUniform2fv(glGetUniformLocation(currentShader->GetProgram(), "cycle"),1,(float*)&juliatime);


	quad->Draw();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/*
	SetCurrentShader(juliaNormShader);
	glBindFramebuffer(GL_FRAMEBUFFER,juliaNormFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), 1.0f/512.0f, 1.0f/512.0f);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), clock->GetTime01());

	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"juliaTex"),12);

	
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D,juliaTex);

	UpdateShaderMatrices();
	
	quad->Draw();


	glEnable(GL_CULL_FACE);
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::FillBuffers() {
	
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();

	glEnable(GL_CULL_FACE);
	
	SetCurrentShader(sceneShader);
	SetShaderLight(*sunLight);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	projMatrix = Matrix4::Perspective(1.0f,10000.0f,
		(float)width / (float)height, 45.0f);

	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	heightMap->Draw();

	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(Vector3(2048.0f,512.0f,2048.0f));
	UpdateShaderMatrices();
	
	glDisable(GL_CULL_FACE);
	groudon->Draw();
	glEnable(GL_CULL_FACE);

	


	//Draw water time!
	SetCurrentShader(waterShader);
	SetShaderLight(*sunLight);
	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(Vector3(-2048,30,-2048));

	UpdateShaderMatrices();
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"juliaTex"),12);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"juliaNormTex"),13);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), clock->GetTime01());
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());
	

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"),9);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_CUBE_MAP,  cubeMap);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex2"),10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP,  cubeMap2);


	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D,juliaTex);

	
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D,juliaNormal);


	waterMap->Draw();



	glBindTexture(GL_TEXTURE_2D, 0);
	
	//Time to draw the sun (and moon)!

	SetCurrentShader(sunShader);
	glDisable(GL_BLEND);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),"time"),sin(clock->GetTime01()* 3.142f));
	glUniform4f(glGetUniformLocation(currentShader->GetProgram(),"unicolour"),1.0f,0.5f,0.5f,1.0f);

	modelMatrix = Matrix4::Translation(sunLight->GetPosition());
	UpdateShaderMatrices();
	sun->Draw();

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),"time"),cos(clock->GetTime01()* 3.142f));
	glUniform4f(glGetUniformLocation(currentShader->GetProgram(),"unicolour"),0.5f,0.5f,1.0f,1.0f);

	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(moonPos);

	UpdateShaderMatrices();
	sun->Draw();

	
	//Time to draw birds
	glDisable(GL_CULL_FACE);
	SetCurrentShader(birdShader);

	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(birdPos) *
				  Matrix4::Rotation(rotation*3.142, Vector3(0.0f,1.0f,0.0f));

	be->SetParticleSize(32.0f);
	be->SetParticleVariance(15.0f);
	be->SetLaunchParticles(8);
	be->SetParticleLifetime(1000.0f);
	be->SetParticleSpeed(1.0f);
	UpdateShaderMatrices();

	
	SetShaderParticleSize(be->GetParticleSize());
	//if(sin(clock->GetTime01() * 3.142f)> 0.5)
	be->Draw();

	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	
}

void Renderer::DrawPointLights() {
	SetCurrentShader(pointlightShader);

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);

	glClearColor(0.15f,0.15f,0.15f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"depthTex"), 4);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"normTex"), 5);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), 1.0f/width, 1.0f/height);

	Vector3 translate = Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 500,
								(RAW_HEIGHT*HEIGHTMAP_Z / 2.0f));
	
	Matrix4 pushMatrix = Matrix4::Translation(translate);
	Matrix4 popMatrix  = Matrix4::Translation(-translate);
	if(sin((clock->GetTime01())* 3.142) < 0.5f)
	for(int x = 0; x < LIGHTNUM*LIGHTNUM; ++x) {
		
		if(sin((clock->GetTime01())* 3.142) < x*0.008) {
			Light &l		= pointLights[x];
			float radius	= l.GetRadius();

			modelMatrix =
				pushMatrix *
				Matrix4::Rotation(rotation, Vector3(0,1,0))*
				popMatrix *
				Matrix4::Translation(l.GetPosition()) *
				Matrix4::Scale(Vector3(radius,radius,radius));
		
			l.SetPosition(modelMatrix.GetPositionVector());

			SetShaderLight(l);
			
			UpdateShaderMatrices();

			float dist =(l.GetPosition()-camera->GetPosition()).Length();
			if(dist < radius) {
				glCullFace(GL_FRONT);
			}
			else {
				glCullFace(GL_BACK);
			}
			sphere->Draw();
		}
	}

	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f,0.2f,0.2f,1);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Renderer::CombineBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, pipFBO);
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(combineShader);



	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),		"diffuseTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),		"emissiveTex"), 4);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),		"specularTex"), 5);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	quad->Draw();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Renderer::ReloadShaders()
{
	ReloadShader(&outputShader);
	ReloadShader(&sceneShader);
	ReloadShader(&combineShader);
	ReloadShader(&pointlightShader);
	ReloadShader(&waterShader);
	ReloadShader(&juliaShader);
	ReloadShader(&skyboxShader);
	ReloadShader(&juliaNormShader);
	ReloadShader(&textShader);
	ReloadShader(&birdShader);
	ReloadShader(&sunShader);
}

bool Renderer::LoadShaders() {
	outputShader = new Shader(SHADERDIR"outputvert.glsl",
							  SHADERDIR"outputfrag.glsl");
	if(!outputShader->LinkProgram())
	{
		return false;
	}

	
	textShader = new Shader(SHADERDIR"TextVertex.glsl",
							 SHADERDIR"TextFragment.glsl");

	if(!textShader->LinkProgram()) {
		return false;
	}
	
	sceneShader = new Shader(SHADERDIR"BumpVertex.glsl",
							 SHADERDIR"bufferFragment.glsl");

	if(!sceneShader->LinkProgram()) {
		return false;
	}

	combineShader = new Shader(SHADERDIR"combinevert.glsl", 
							  SHADERDIR"combinefrag.glsl");
	if(!combineShader->LinkProgram()) {
		return false;
	}

	pointlightShader = new Shader(SHADERDIR"pointlightvert.glsl",
								  SHADERDIR"pointlightfrag.glsl");

	if(!pointlightShader->LinkProgram()) {
		return false;
	}


	waterShader = new Shader(SHADERDIR"waterVertex.glsl",
							 SHADERDIR"superwaterfragment.glsl");

	if(!waterShader->LinkProgram()) {
		return false;
	}

	juliaShader = new Shader(SHADERDIR"jvert.glsl",
							 SHADERDIR"jfrag.glsl");

	if(!juliaShader->LinkProgram()) {
		return false;
	}

	juliaNormShader = new Shader(SHADERDIR"waternormVert.glsl",
							     SHADERDIR"waternormFrag.glsl");

	if(!juliaNormShader->LinkProgram()) {
		return false;
	}

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
							  SHADERDIR"skyboxFragmentcw.glsl");

	if(!skyboxShader->LinkProgram()) {
		return false;
	}

	
	sunShader =   new Shader(SHADERDIR"sunmoonvert.glsl",
							  SHADERDIR"sunmoonfrag.glsl");

	if(!sunShader->LinkProgram()) {
		return false;
	}


	
	birdShader =   new Shader(SHADERDIR"particleVertex.glsl",
							  SHADERDIR"particleFragment.glsl",
							  SHADERDIR"particlegeometry.glsl");

	if(!birdShader->LinkProgram()) {
		return false;
	}


	return true;
}

void Renderer::ReloadShader(Shader ** s) {
	Shader* temp = new Shader(*s);

	if(!temp->LinkProgram())
	{
		cout << (*s)->fFile << "  didnt load" <<endl;

		delete temp;
	}
	else
	{
		delete *s;
		
		*s = temp;
	}
}

bool Renderer::GenerateBuffers() {
	
	glGenFramebuffers(1,&bufferFBO);
	glGenFramebuffers(1,&pointLightFBO);

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	GenerateScreenTexture(bufferDepthTex,   true);
	GenerateScreenTexture(bufferColourTex,  false);
	GenerateScreenTexture(bufferNormalTex,  false);
	GenerateScreenTexture(lightEmissiveTex, false);
	GenerateScreenTexture(lightSpecularTex, false);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,		GL_TEXTURE_2D, bufferColourTex,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,		GL_TEXTURE_2D, bufferNormalTex,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,			GL_TEXTURE_2D, bufferDepthTex,0);
	glDrawBuffers(2, buffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,		GL_TEXTURE_2D, lightEmissiveTex,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	
	if(!GeneratePostProcessRenderTarget(juliaFBO, juliaTex, 512, 512))
	{
		return false;
	}
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=	GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}
	
	
	if(!GeneratePostProcessRenderTarget(juliaFBO, juliaTex, 512, 512))
	{
		return false;
	}
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=	GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}

	if(!GeneratePostProcessRenderTarget(juliaNormFBO, juliaNormal, 512, 512))
	{
		return false;
	}
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=	GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}


	if(!GeneratePostProcessRenderTarget(pipFBO, pipColourTex, width, height))
	{
		return false;
	}


	return true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

bool Renderer::GeneratePostProcessRenderTarget(GLuint &framebuffer, GLuint &colourTex, int width, int height){
	glGenTextures(1, &colourTex);
	glBindTexture(GL_TEXTURE_2D, colourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D,0);



	glGenFramebuffers(1,&framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, colourTex,0);


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=	GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	return true;

}

bool Renderer::LoadResources() {
	quad = Mesh::GenerateQuad();

	heightMap		= new HeightMap(TEXTUREDIR"island.raw");
	heightMap->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR"grass.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR"grassbump.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	sphere = new OBJMesh();
	if(!sphere->LoadOBJMesh(MESHDIR"ico2.obj")) {
		return false;
	}

	waterMap = new HeightMap();
	waterMap->SetTexture(
		SOIL_load_OGL_texture(
		TEXTUREDIR"water.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	waterMap->SetBumpMap(
		SOIL_load_OGL_texture(
		TEXTUREDIR"waterbump.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	SetTextureRepeating(waterMap->GetTexture(), true);
	SetTextureRepeating(waterMap->GetBumpMap(), true);

	groudon = new OBJMesh();
	if(!groudon->LoadOBJMesh(MESHDIR"groudon.obj")) {
		return false;
	}
	groudon->SetTexture(
		SOIL_load_OGL_texture(
		TEXTUREDIR"swirl.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	groudon->SetBumpMap(
		SOIL_load_OGL_texture(
		TEXTUREDIR"Barren RedsDOT3.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));


	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"interstellar_ft.tga", TEXTUREDIR"interstellar_bk.tga",
		TEXTUREDIR"interstellar_up.tga", TEXTUREDIR"interstellar_dn.tga",
		TEXTUREDIR"interstellar_rt.tga", TEXTUREDIR"interstellar_lf.tga", 
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,0);


	cubeMap2 = SOIL_load_OGL_cubemap(
		TEXTUREDIR"grimmnight_ft.tga", TEXTUREDIR"grimmnight_bk.tga",
		TEXTUREDIR"grimmnight_up.tga", TEXTUREDIR"grimmnight_dn.tga",
		TEXTUREDIR"grimmnight_rt.tga", TEXTUREDIR"grimmnight_lf.tga", 
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,0);

	sun = new OBJMesh();
	if(!sun->LoadOBJMesh(MESHDIR"sun.obj")) {
		return false;
	}
	sun->SetTexture(
		SOIL_load_OGL_texture(
		TEXTUREDIR"dissolve.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	basicFont = new Font(SOIL_load_OGL_texture(
		TEXTUREDIR"tahoma2.tga" , SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT),16,16);


	if(!cubeMap || !cubeMap2 || !waterMap->GetTexture() || !waterMap->GetBumpMap() || 
		!heightMap->GetTexture() || !heightMap->GetBumpMap() || 
		!groudon->GetTexture() || !groudon->GetBumpMap() ||
		!sun->GetTexture()){
			return false;
	}


	be = new BirdEmitter();






	return true;
}