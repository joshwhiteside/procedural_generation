#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/Camera.h"


#define POST_PASSES 10

class Renderer : public OGLRenderer		{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void		PresentScene(GLuint textureToDisplay, bool backBuffer = true);
	void		DrawPostProcess();
	void		DrawScene(bool backBuffer = false);

	void		DrawDepthBuffer();

	Shader*		sceneShader;
	Shader*		processShader;
	Shader*		depthShader;

	Camera*		camera;

	Mesh*		quad;
	HeightMap*	heightMap;

	GLuint		sobelTest;


	GLuint		bufferFBO;
	GLuint		processFBO;
	GLuint		bufferColourTex[2];
	GLuint		bufferDepthTex;

	GLuint		depthTex;

	GLuint		sceneColourTex[2];
	GLuint		sceneDepthTex;

	bool		postProcessOn;

};