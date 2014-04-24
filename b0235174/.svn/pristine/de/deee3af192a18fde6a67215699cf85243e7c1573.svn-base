#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
	camera = new Camera(0.0f, 0.0f, Vector3(
		RAW_WIDTH * HEIGHTMAP_X /2.0f, 500, RAW_HEIGHT*HEIGHTMAP_Z));

	cycle = 0;


	heightMap		= new HeightMap(TEXTUREDIR"terrain.raw");
	currentShader	= new Shader(SHADERDIR"PerPixelVertex.glsl",
								 SHADERDIR"PerPixelFragment.glsl");

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if(!currentShader->LinkProgram() || !heightMap->GetTexture()) {
		return;
	}

	SetTextureRepeating(heightMap->GetTexture(), true);


	for(int i = 0; i<4; ++i) {
		/*
		lights[i] = new Light();
		lights[i]->SetPosition(Vector3(250.0f*i,500,250.0f*i));
		lights[i]->SetColour(Vector4( (float)(rand()%255 / 255),(float)(rand()%255 / 255),(float)(rand()%255 / 255),1.0f));
		*/

			lights[i] = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X /2.0f),
				500.0f,(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
				Vector4(i%2,i%3,i%4,1), (RAW_WIDTH*HEIGHTMAP_X) /2.0f);

			lights[i]->SetPosition(Vector3(1000.0f*i ,500.0f,1000.0f*i));
	}

	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X /2.0f),
				500.0f,(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
				Vector4(1,1,1,1), (RAW_WIDTH*HEIGHTMAP_X) /2.0f);

	projMatrix = Matrix4::Perspective(1.0f,15000.0f,
				 (float)width/ (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete heightMap;
	delete light;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix	= camera->BuildViewMatrix();
}

void Renderer::RenderScene() {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
								     "diffuseTex") , 0);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	UpdateShaderMatrices();

	
	//SetShaderLight(*lights[(++cycle)%4]);

	SetShaderLight(*lights[1]);

	heightMap->Draw();

	glUseProgram(0);

	SwapBuffers();
}