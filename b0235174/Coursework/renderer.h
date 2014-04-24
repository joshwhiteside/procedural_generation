#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/heightmap.h"
#include "../../nclgl/TextMesh.h"
#include "../../nclgl/BirdEmitter.h"

#include "DayClock.h"

#define LIGHTNUM 8
#define NIGHT_TIME 3600
#define DAY_RADIUS  50000.0f

using namespace std;

class Renderer : public OGLRenderer		{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	float		fps;

protected:
	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();
	void DrawStuff();
	bool LoadShaders();
	void DrawSkybox();
	void ShowScene();
	void DrawJuliaSet();
	void RenderText();
	void DrawText(const std::string &text, const Vector3 &position, const float size);
	
	void ReloadShader(Shader ** s);
	void ReloadShaders();


	bool GenerateBuffers();
	bool GeneratePostProcessRenderTarget(GLuint &framebuffer, GLuint &colourTex, int width, int height);
	bool LoadResources();

	Shader*		sceneShader;
	Shader*		pointlightShader;
	Shader*		combineShader;
	Shader*		debugShader;
	Shader*		outputShader;
	Shader*		waterShader;
	Shader*		juliaShader;
	Shader*		skyboxShader;
	Shader*		juliaNormShader;
	Shader*		birdShader;
	Shader*		textShader;
	Shader*		sunShader;

	Font*		basicFont;

	BirdEmitter* be;

	Vector3		moonPos;
	Vector3		birdPos;

	Light*		pointLights;
	Light*		sunLight;
	Mesh*		heightMap;
	Mesh*		waterMap;
	OBJMesh*	sphere;
	Mesh*		quad;
	//Mesh*		skyquad;
	Mesh*		outputquad;
	Camera*		camera;
	OBJMesh*	groudon;
	OBJMesh*	sun;
	//OBJMesh*	birds;

	float		rotation;

	GLuint		cubeMap;
	GLuint		cubeMap2;

	GLuint		bufferFBO;
	GLuint		bufferColourTex;
	GLuint		bufferNormalTex;
	GLuint		bufferDepthTex;

	GLuint		pointLightFBO;
	GLuint		lightEmissiveTex;
	GLuint		lightSpecularTex;



	GLuint		juliaFBO;
	GLuint		juliaTex;

	GLuint		juliaNormFBO;
	GLuint		juliaNormal;

	GLuint		pipFBO;
	GLuint		pipColourTex;

	float	time;
	Vector2 juliatime;
	float	speed;

	DayClock *clock;


};