#pragma once

#include "../nclgl/Mesh.h"

#define TERRAIN_GRID_WIDTH 32
#define TERRAIN_GRID_HEIGHT 32
#define TERRAIN_GRID_DEPTH 128

#define TERRAIN_SPACING_X 1
#define TERRAIN_SPACING_Y 1
#define TERRAIN_SPACING_Z 1


class TerrainChunk : public Mesh {
public:
	TerrainChunk(Vector3 wPos);
	~TerrainChunk();


	static GLuint noiseTex;
protected:
	Vector3* worldPos;


};