#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/Camera.h"

#define POST_PASSES 3

class Renderer : public OGLRenderer		{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void PresentScene();
	void DrawPostProcess();
	void DrawDepthBuffer();
	void DrawDepthofField();
	void DrawScene();
	void DrawJulia();

	Shader* sceneShader;
	Shader* processShader;
	Shader* depthShader;
	Shader* dofShader;
	Shader* juliaShader;


	Camera* camera;

	Mesh*	quad;
	HeightMap* heightMap;

	
	GLuint	bufferFBO;
	GLuint	processFBO;
	GLuint	bufferColourTex[3];
	GLuint	bufferDepthTex;
	GLuint  juliaTex;

	GLuint  jTex;


	Vector2 cycle;
	//GLuint  origTex;
};