#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/heightmap.h"
#include "../../nclgl/TextMesh.h"
#include "noisefunc.h"

using namespace std;


class Renderer : public OGLRenderer		{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void reloadShaders();
	bool loadResources();

	Mesh*	testSquare;

	Shader* texturedShader;

	GLuint noise64;

	Camera*		camera;

};
