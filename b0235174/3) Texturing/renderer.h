#pragma once

#include "./nclgl/OGLRenderer.h"
#include "./nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	void UpdateTextureMatrix(float rotation);
	void ScaleTextureMatrix(float scale);
	void ToggleRepeating();
	void ToggleFiltering();
	void ToggleAnsiotropy();

	virtual void UpdateScene(float msec);

protected:
	Mesh* triangle;

	Camera* camera;

	bool filtering;
	bool repeating;
};