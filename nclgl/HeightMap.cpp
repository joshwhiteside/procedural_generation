#include "HeightMap.h"

HeightMap::HeightMap(std::string name)	{
	std::ifstream file(name.c_str(), ios::binary);

	if(!file) {
		return;
	}
	numVertices		= RAW_WIDTH*RAW_HEIGHT;
	numIndices		= (RAW_WIDTH-1)*(RAW_HEIGHT-1)*6;
	colours			= new Vector4[numVertices];
	vertices		= new Vector3[numVertices];
	textureCoords	= new Vector2[numVertices];
	indices			= new GLuint[numIndices];


	unsigned char*data	= new unsigned char[numVertices];
	file.read((char*)data,numVertices*sizeof(unsigned char));
	file.close();

	for(int x = 0; x <RAW_WIDTH; ++x)	{
		for(int z = 0; z < RAW_HEIGHT; ++z) {
			int offset = (x * RAW_WIDTH) + z;

			colours[offset] = Vector4(1,1,1,1);
			vertices[offset]		= Vector3(
			x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);
			
			textureCoords[offset]	= Vector2(
			x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}

	delete data;

	numIndices = 0;

	for(int x = 0; x < RAW_WIDTH-1; ++x)	{
		for(int z = 0; z < RAW_HEIGHT-1; ++z)	{
			int a = (x		* (RAW_WIDTH)) + z;
			int b = ((x+1)  * (RAW_WIDTH)) + z;
			int c = ((x+1)  * (RAW_WIDTH)) + (z+1);
			int d = (x		* (RAW_WIDTH)) + (z+1);
		
			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;

		}
	}
	GenerateNormals();
	GenerateTangents();
	GenerateColours();
	BufferData();
}


HeightMap::HeightMap() {

	numVertices		= (WATER_WIDTH)*(WATER_HEIGHT);
	numIndices		= (WATER_WIDTH-1)*(WATER_HEIGHT-1)*6;
	colours			= new Vector4[numVertices];
	vertices		= new Vector3[numVertices];
	textureCoords	= new Vector2[numVertices];
	indices			= new GLuint[numIndices];


	for(int x = 0; x <WATER_WIDTH; ++x)	{
		for(int z = 0; z < WATER_HEIGHT; ++z) {
			int offset = (x * WATER_WIDTH) + z;

			colours[offset] = Vector4(1,1,1,1);
			vertices[offset]		= Vector3(
			x * HEIGHTMAP_X, 60.0f, z * HEIGHTMAP_Z);
			
			textureCoords[offset]	= Vector2(
			x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}

		numIndices = 0;

	for(int x = 0; x < WATER_WIDTH-1; ++x)	{
		for(int z = 0; z < WATER_HEIGHT-1; ++z)	{
			int a = (x		* (WATER_WIDTH)) + z;
			int b = ((x+1)  * (WATER_WIDTH)) + z;
			int c = ((x+1)  * (WATER_WIDTH)) + (z+1);
			int d = (x		* (WATER_WIDTH)) + (z+1);
		
			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;

		}
	}
	GenerateNormals();
	GenerateTangents();
	GenerateColours();
	BufferData();
}