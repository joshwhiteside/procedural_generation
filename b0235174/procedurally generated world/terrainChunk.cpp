#include "TerrainChunk.h"

TerrainChunk::TerrainChunk(Vector3 wPos) {
	worldPos = wPos;

	indices			= NULL;
	normals			= NULL;
	textureCoords	= NULL;
	numIndices		= NULL;

	GenerateNewChunk(wPos);
	
}


void TerrainChunk::GenerateNewChunk(Vector3 wPos) {
	worldPos = wPos;

	generateNoiseForChunk();
	Vector4 cols[7];

	cols[0] = Vector4(1.0f,0.0f,0.0f,1.0f);
	cols[1] = Vector4(0.0f,1.0f,0.0f,1.0f);
	cols[2] = Vector4(0.0f,0.0f,1.0f,1.0f);
	cols[3] = Vector4(1.0f,1.0f,1.0f,1.0f);
	cols[4] = Vector4(1.0f,0.0f,1.0f,1.0f);
	cols[5] = Vector4(1.0f,1.0f,0.0f,1.0f);
	cols[6] = Vector4(0.0f,1.0f,1.0f,1.0f);
	int col = 0;

	//Vertices
	vector<Vector3> genVertices;
	vector<Vector4> genColours;
	numVertices = 0;

	for(int x=0; x < TERRAIN_GRID_WIDTH; x++) {
		for(int y = 0; y < TERRAIN_GRID_HEIGHT; y++){
			for(int z = 0; z < TERRAIN_GRID_DEPTH; z++) {
				Vector3 currentCellPos = Vector3(x * TERRAIN_SPACING_X, y * TERRAIN_SPACING_Y, z * TERRAIN_SPACING_Z); //This is ok..
				
				int caseNo = getCaseNumber(x,y,z);
				//if(caseNo!=0 && caseNo!= 255) caseNo=51;
				
				int numNewVertices = getNumVerticesFromCase(caseNo);

					//= caseToNumPolys[caseNo];
				//col= (col+1) % 5;
				col = rand()% 7;
				for(int i = 0; i < numNewVertices; i++){
					//Puts the new vertex in the right position in terms of the entire grid.
					Vector3 newVertex = currentCellPos;
					
					newVertex += edges[triTable[caseNo][i]]; //Grim

					genVertices.push_back(newVertex);
					genColours.push_back(cols[col]);

				}


			}
		}
	}
	numVertices = genVertices.size();
	
	vertices = new Vector3[numVertices];
	colours  = new Vector4[numVertices];

	int index = 0;
	vector<Vector4>::iterator coli = genColours.begin();
	for(vector<Vector3>::iterator i = genVertices.begin(); i != genVertices.end(); i++){
		vertices[index] = (*i);
		colours[index] = (*coli);
		coli++;
		/*
		float r,g,b;
		r = (float)(rand()%100) * 0.01f;
		g = (float)(rand()%100) * 0.01f;
		b = (float)(rand()%100) * 0.01f;
		colours[index] = Vector4(r,g,b,1.0f);
		*/
		index++;
	}

	cout << "NUMBER OF VERTICES: " << numVertices << endl;
	
	type = GL_TRIANGLES;

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
	

		//FLOOR + CEILING TEST
		//if(val < 0.5) caseNo |= valToOr;

		//NOISE TEST
		//WHO KNOWS WHY THE RANGE OF THE NOISE IS KINDA IN THE RANGE -6 to 6 :'( 
		
		if(val > 0.0) caseNo |= valToOr;

		valToOr *= 2;

	}

	return caseNo;
}

//Temp..
int TerrainChunk::getNumVerticesFromCase(int caseNo){
	int retValue = 0;

	for(retValue = 0; retValue < 16; retValue++){
		if(triTable[caseNo][retValue]==-1) break;
	}
	
	return retValue;
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
				double noiseInput = -1;
				//OCTAVE AND PERSISTANCE TEMPORARY.
				//if(y==0) noiseInput = 1;
				//if(sin(samplePoint.x)>0.0f && sin(samplePoint.y)>0.0f && (samplePoint.y < 13)) noiseInput = 1;
				//if(y==1 || y == 31) noiseInput = 1;
				//if(x%8==0 || y%8==0 || z%8==0) noiseInput =1 ;
				
				//noiseInput = NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,1.0);
				if(y<8) noiseInput =1;
				noiseInput += NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,1.0);
				
				noise[x][y][z] = noiseInput;
				if(noiseInput > Highest) Highest = noiseInput;
				if(noiseInput < Lowest)  Lowest = noiseInput;
				
			}
		}
	}

	cout << "Lowest :" <<Lowest	 << endl;
	cout << "Highest :"<<Highest << endl;
}
