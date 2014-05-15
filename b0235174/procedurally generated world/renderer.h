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
const float LIGHT_RADIUS = 5000.0f;
#define TERRAIN_DRAW_SIZE 25.0f
#define CHUNK_LOAD_RADIUS 6
#define CHUNK_GRID_SIZE 9

const float F_TIMER_MAX =  128.0f * 3.142f;

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
	GLuint cloudTex;

	Camera*		camera;
	float cameraGridX, cameraGridZ;

	float zTimer,fTimer,cTimer;
	
	void updateTerrain();
	void RequestNewTerrain(int x, int y);
	TerrainChunk* findFurthestChunk();


	vector<TerrainChunk*> testChunk;
};
