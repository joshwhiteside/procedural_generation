#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../../nclgl/Julia.h"

class Renderer : public OGLRenderer	{
public:
	Renderer(Window & parent);
	virtual ~Renderer (void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	

protected:
	Shader* jShader;

	GLuint texture;
	JuliaSet* quad;
};