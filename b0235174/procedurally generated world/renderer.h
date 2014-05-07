#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/heightmap.h"
#include "../../nclgl/TextMesh.h"
#include "noisefunc.h"
#include "TerrainChunk.h"

using namespace std;

const float DRAW_DISTANCE = 10000.0f;
#define TERRAIN_DRAW_SIZE 25.0f

class Renderer : public OGLRenderer		{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void RenderNoiseCheck();
	void RenderSky();

protected:
	void reloadShaders();
	bool loadResources();

	Mesh*	testSquare;
	Mesh*	skyquad;

	Shader* texturedShader;
	Shader* chunkShader;
	Shader* cloudShader;
	Shader* skyShader;

	Light* light;

	GLuint noise64;

	Camera*		camera;
	float cameraGridX, cameraGridZ;

	float zTimer;
	
	void updateTerrain();
	void RequestNewTerrain(int x, int y);
	TerrainChunk* findFurthestChunk();


	vector<TerrainChunk*> testChunk;
};
