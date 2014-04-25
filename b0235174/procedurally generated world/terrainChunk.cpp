#include "TerrainChunk.h"

TerrainChunk::TerrainChunk(Vector3 wPos) {
	worldPos = new Vector3(wPos);

	indices			= NULL;
	normals			= NULL;
	textureCoords	= NULL;
	numIndices		= NULL;

	numVertices		= TERRAIN_GRID_WIDTH * TERRAIN_GRID_HEIGHT * TERRAIN_GRID_DEPTH;
	vertices		= new Vector3[numVertices];

	for(int x=0; x < TERRAIN_GRID_WIDTH; x++) {
		for(int y = 0; y < TERRAIN_GRID_HEIGHT; y++){
			for(int z = 0; z < TERRAIN_GRID_DEPTH; z++) {
				int place;
				place = x;
				place += y * 32;
				place += z * 32 * 32;
				
				vertices[place] = Vector3(x * TERRAIN_SPACING_X, y * TERRAIN_SPACING_Y, z * TERRAIN_SPACING_Z);

			}
		}
	}


	BufferData();
	type = GL_POINTS;
}