#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/heightmap.h"

#define LIGHTNUM 8

class Renderer : public OGLRenderer		{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);
protected:
	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();
	void DrawStuff();

	//Make a new texture

	void GenerateScreenTexture(GLuint &into, bool depth = false);

	Shader*		sceneShader;
	Shader*		pointlightShader;
	Shader*		combineShader;
	Shader*		debugShader;

	Light*		pointLights;
	Mesh*		heightMap;
	OBJMesh*	sphere;
	Mesh*		quad;
	Camera*		camera;

	float		rotation;

	GLuint		bufferFBO;
	GLuint		bufferColourTex;
	GLuint		bufferNormalTex;
	GLuint		bufferDepthTex;

	GLuint		pointLightFBO;
	GLuint		lightEmissiveTex;
	GLuint		lightSpecularTex;

	int no;


};