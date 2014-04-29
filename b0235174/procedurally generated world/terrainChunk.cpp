#include "TerrainChunk.h"

TerrainChunk::TerrainChunk(Vector3 wPos) {
	worldPos = wPos;

	indices			= NULL;
	normals			= NULL;
	textureCoords	= NULL;
	numIndices		= NULL;

	GenerateNewChunk(wPos);
	/*
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

	*/
}


void TerrainChunk::GenerateNewChunk(Vector3 wPos) {
	worldPos = wPos;

	generateNoiseForChunk();


	//Get number of vertices for this case
	//Iterate through these number of vertices



	//Vertices
	vector<Vector3> genVertices;
	numVertices = 0;

	for(int x=0; x < TERRAIN_GRID_WIDTH; x++) {
		for(int y = 0; y < TERRAIN_GRID_HEIGHT; y++){
			for(int z = 0; z < TERRAIN_GRID_DEPTH; z++) {
				Vector3 currentCellPos = Vector3(x * TERRAIN_SPACING_X, y * TERRAIN_SPACING_Y, z * TERRAIN_SPACING_Z);
				
			
				int caseNo = getCaseNumber(x,y,z);
				int numNewVertices = caseToNumPolys[caseNo];
				
				int index = 0;
				for(int i = 0; i < numNewVertices; i++){
					//Puts the new vertex in the right position in terms of the entire grid.
					Vector3 newVertex = currentCellPos;
					
					newVertex += edges[triTable[caseNo][i]]; //Grim

					genVertices.push_back(newVertex);

				}


			}
		}
	}
	numVertices = genVertices.size();
	
	vertices = new Vector3[numVertices];
	for(unsigned int j = 0; j < numVertices; ++j) {
				vertices[j] = genVertices[j];
	}
	

	cout << "NUMBER OF VERTICES: " << numVertices << endl;
	
	GenerateNormals();

	BufferData();
}

int TerrainChunk::getCaseNumber(int x, int y, int z){
	int caseNo = 0;
	int valToOr = 1;
	for(int i = 0; i < 8; i++){
		int toCheckX = x + gridCheckPoints[i].x;
		int toCheckY = y + gridCheckPoints[i].y;
		int toCheckZ = z + gridCheckPoints[i].z;

		float val = noise[toCheckX][toCheckY][toCheckZ];
	
		//WHO KNOWS WHY THE RANGE OF THE NOISE IS KINDA IN THE RANGE -6 to 6 :'( 
		if(val < 0.5) caseNo |= valToOr;

		valToOr *= 2;

	}

	return caseNo;
}

void TerrainChunk::generateNoiseForChunk(){
	double Lowest = 0;
	double Highest= 0;

	for(int x = 0; x < TERRAIN_NOISE_SIZE; x++){
		for(int y = 0; y < TERRAIN_NOISE_SIZE; y++){
			for(int z = 0; z < TERRAIN_NOISE_SIZE; z++){
				Vector3 currentCellPos = Vector3(x * TERRAIN_SPACING_X, y * TERRAIN_SPACING_Y, z * TERRAIN_SPACING_Z);
				Vector3 samplePoint = (currentCellPos + worldPos);

				//TEMP
				double noiseInput = 0;
				//OCTAVE AND PERSISTANCE TEMPORARY.
				if(y==0 || y == 32) noiseInput = 1;

				//double noiseInput = NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,1.0);
				noise[x][y][z] = noiseInput;
				if(noiseInput > Highest) Highest = noiseInput;
				if(noiseInput < Lowest)  Lowest = noiseInput;
				
			}
		}
	}

	cout << "Lowest :" <<Lowest	 << endl;
	cout << "Highest :"<<Highest << endl;
}
