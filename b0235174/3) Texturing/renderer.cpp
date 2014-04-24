#include "renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();

	triangle ->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	triangle ->SetTexture2(SOIL_load_OGL_texture(TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));


	if(!triangle->GetTexture() || !triangle->GetTexture2()) {
		return;
	}

	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl",
							   SHADERDIR"texturedfragment.glsl");

	camera = new Camera();

	if(!currentShader->LinkProgram()) {
		return;
	}

	init = true;

	projMatrix = Matrix4::Perspective(1.0f,10000.0f, (float) width / (float) height, 45.0f);

	filtering = true;
	repeating = true;
}

Renderer::~Renderer(void)	{
	delete triangle;
	delete camera;
}

void Renderer::UpdateTextureMatrix(float value)	{
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f,0.5f,0));
	Matrix4 popPos  = Matrix4::Translation(Vector3(-0.5f,-0.5f,0));
	Matrix4 rotation= Matrix4::Rotation(value, Vector3(0,0,1));
	textureMatrix	= pushPos * rotation * popPos;
}

void Renderer::ScaleTextureMatrix(float value)	{
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f,0.5f,0));
	Matrix4 popPos  = Matrix4::Translation(Vector3(-0.5f,-0.5f,0));
	Matrix4 rotation= Matrix4::Scale(Vector3(value,value,1));
	textureMatrix	= pushPos * rotation * popPos;
}

void Renderer::ToggleRepeating()	{
	repeating  = !repeating;
	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
					repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
					repeating ? GL_REPEAT : GL_CLAMP);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture2());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
					repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
					repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ToggleFiltering()	{
	filtering  = !filtering;
	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					repeating ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					repeating ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture2());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					repeating ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					repeating ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ToggleAnsiotropy()	{
	GLfloat maximumAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);

	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnisotropy);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );

	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture2());
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnisotropy);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );


	glBindTexture(GL_TEXTURE_2D,0);

}

void Renderer::RenderScene()	{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());

	modelMatrix.Scale(Vector3(100.0f,100.0f,100.0f));
	

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), 
		"projMatrix"), 1, false, (float*)&projMatrix);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),
		"viewMatrix"), 1, false, (float*) &viewMatrix);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),
		"modelMatrix"), 1, false, (float*) &modelMatrix);



	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"),0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "secondTex"),1);

	
            //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	triangle->Draw();

	glUseProgram(0);
	SwapBuffers();
}


void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix	= camera->BuildViewMatrix();
}