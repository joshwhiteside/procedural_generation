#include "TerrainChunk.h"

double TerrainChunk::noiseLayer1[NOISE_LAYER_1][NOISE_LAYER_1][NOISE_LAYER_1];
double TerrainChunk::noiseLayer2[NOISE_LAYER_2][NOISE_LAYER_2][NOISE_LAYER_2];
double TerrainChunk::noiseLayer3[NOISE_LAYER_3][NOISE_LAYER_3][NOISE_LAYER_3];


int TerrainChunk::toGridPos(float n){
	return (int) ((n / TERRAIN_SPACING_X) / TERRAIN_GRID_WIDTH);
}


TerrainChunk::TerrainChunk(int x, int z) {
	
	indices			= NULL;
	textureCoords	= NULL;
	numIndices		= NULL;

	reassign(x ,z);
}


TerrainChunk::TerrainChunk(Vector3 wPos) {
	worldPos = wPos;

	indices			= NULL;
	normals			= NULL;
	textureCoords	= NULL;
	numIndices		= NULL;

	GenerateNewChunk(wPos);
	
}

bool TerrainChunk::isSameGridPos(int x, int z){
	return (x==gridX && z==gridZ);
}

void TerrainChunk::reassign(int x, int z){
	gridX = x;
	gridZ = z;

	Vector3 wPos = Vector3();

	wPos.x = x * TERRAIN_SPACING_X * TERRAIN_GRID_WIDTH;
	wPos.z = z * TERRAIN_SPACING_Z * TERRAIN_GRID_DEPTH;
	
	GenerateNewChunk(wPos);
}

void TerrainChunk::GenerateNewChunk(Vector3 wPos) {
	//Used for model matrix :D
	worldPos = wPos;

	//Clean stuff up.
	delete[] vertices;
	delete[] colours;
	delete[] normals;

	glDeleteBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glDeleteBuffers(1, &bufferObject[COLOUR_BUFFER]);
	glDeleteBuffers(1, &bufferObject[NORMAL_BUFFER]);
	

	//Hopefully we can get rid of this soon.
	generateNoiseForChunk();

	//This whole col set up is for debugging. 
	Vector4 cols[7];

	cols[0] = Vector4(1.0f,0.0f,0.0f,1.0f);
	cols[1] = Vector4(0.0f,1.0f,0.0f,1.0f);
	cols[2] = Vector4(0.0f,0.0f,1.0f,1.0f);
	cols[3] = Vector4(1.0f,1.0f,1.0f,1.0f);
	cols[4] = Vector4(1.0f,0.0f,1.0f,1.0f);
	cols[5] = Vector4(1.0f,1.0f,0.0f,1.0f);
	cols[6] = Vector4(0.0f,1.0f,1.0f,1.0f);
	int col = 0;

	//Vectors that hold all the new vertex info.
	vector<Vector3> genVertices;
	vector<Vector3> genNormals;
	vector<Vector4> genColours;
	numVertices = 0;

	//Its like a 3d heightmap.
	for(int x=0; x < TERRAIN_GRID_WIDTH; x++) {
		for(int y = 0; y < TERRAIN_GRID_HEIGHT; y++){
			for(int z = 0; z < TERRAIN_GRID_DEPTH; z++) {
				Vector3 currentCellPos = Vector3(x * TERRAIN_SPACING_X, y * TERRAIN_SPACING_Y, z * TERRAIN_SPACING_Z); //This is ok
				
				int caseNo = getCaseNumber(x,y,z);
				
				int numNewVertices = getNumVerticesFromCase(caseNo);

				//Every set of triangles generated is a random colour.
				col = rand()% 7;

				Vector3 normalToInsert;
				
				for(int i = 0; i < numNewVertices; i++){
					//Compute normal for each triangle
					if(i%3==0){
						Vector3 &a = worldPos + currentCellPos +  edges[triTable[caseNo][i]];
						Vector3 &b = worldPos + currentCellPos +  edges[triTable[caseNo][i+1]];
						Vector3 &c = worldPos + currentCellPos +  edges[triTable[caseNo][i+2]];

						normalToInsert = Vector3::Cross(b-a,c-a);
					
						normalToInsert.Normalise();
					}

					//Puts the new vertex in the right position in terms of the entire grid.
					Vector3 newVertex = currentCellPos;
					
					newVertex += edges[triTable[caseNo][i]]; //Grim

					genVertices.push_back(newVertex);
					genColours.push_back(cols[col]);
					genNormals.push_back(normalToInsert);

				}


			}
		}
	}
	numVertices = genVertices.size();
	
	vertices = new Vector3[numVertices];
	normals  = new Vector3[numVertices];
	colours  = new Vector4[numVertices];

	int index = 0;
	vector<Vector4>::iterator coli = genColours.begin();
	vector<Vector3>::iterator normi = genNormals.begin();
	for(vector<Vector3>::iterator i = genVertices.begin(); i != genVertices.end(); i++){
		vertices[index] = (*i);
		colours[index] = (*coli);
		normals[index] = (*normi);
		coli++; normi++;
		index++;
	}

	cout << "NUMBER OF VERTICES: " << numVertices << endl;
	
	type = GL_TRIANGLES;

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
		
		if(val > 0.5) caseNo |= valToOr;

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
				double noiseInput = 0;
				//OCTAVE AND PERSISTANCE TEMPORARY.
				//if(y==0) noiseInput = 1;
				//if(sin(samplePoint.x)>0.0f && sin(samplePoint.y)>0.0f && (samplePoint.y < 13)) noiseInput = 1;
				//if(y==1 || y == 31) noiseInput = 1;
				//if(x%8==0 || y%8==0 || z%8==0) noiseInput =1 ;
				
				//noiseInput = NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,1.0);
				
				if(y<1) noiseInput =1;
				/*	
				noiseInput += NoiseFunc::perlinNoise3D(pow(samplePoint.x,2),sin(samplePoint.y),samplePoint.z,8,0.25) * 0.25;
				noiseInput += NoiseFunc::perlinNoise3D(samplePoint.x,cos(samplePoint.y),tan(samplePoint.z),8,0.25) * 0.5;
				noiseInput += NoiseFunc::perlinNoise3D(tan(samplePoint.x),(samplePoint.y),(samplePoint.z),8,0.25) * 0.25;
				*/
				
				/*
				noiseInput  += NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,0.25) * sin((samplePoint.x +samplePoint.z) * 0.00001);



				noiseInput  += NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,0.25) * 0.25;

				
				samplePoint = samplePoint * 1.54;

				noiseInput  += NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,0.25) * 0.125;

				
				samplePoint = samplePoint * 1.16;

				noiseInput  += NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,0.25) * 0.0625;

				
				samplePoint = samplePoint * 1.17;

				noiseInput  += NoiseFunc::perlinNoise3D(samplePoint.x,samplePoint.y,samplePoint.z,8,0.25) * 0.03125;

				*/

				noiseInput += fetchNoise(samplePoint);

				if(y==32) noiseInput = -1;
			

				noise[x][y][z] = noiseInput;
				if(noiseInput > Highest) Highest = noiseInput;
				if(noiseInput < Lowest)  Lowest = noiseInput;
				
			}
		}
	}

	cout << "Lowest :" <<Lowest	 << endl;
	cout << "Highest :"<<Highest << endl;
}

void TerrainChunk::BufferData()	{
	glBindVertexArray(arrayObject);
	glGenBuffers(1,&bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3),
		vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3 , GL_FLOAT, GL_FALSE, 0,0);

	glEnableVertexAttribArray(VERTEX_BUFFER);

	if(textureCoords)	{
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector2),
			textureCoords, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER,2,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	if(colours)		{
		glGenBuffers(1,&bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector4),
			colours, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER,4,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	if(indices)	{
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint),
			indices, GL_DYNAMIC_DRAW);
	}

	if(normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3),
			normals, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);

	}

	if(tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3),
			tangents, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}

	glBindVertexArray(0);
}

void TerrainChunk::initNoiseLayers(){
	srand(time(NULL));
	
	Vector3 samplePoint;
	samplePoint.x = (double)(rand()%10000 * 0.001) * (rand()%1000);
	samplePoint.y = (double)(rand()%10000 * 0.001) * (rand()%1000);
	samplePoint.z = (double)(rand()%10000 * 0.001) * (rand()%1000);

	for(int x = 0; x < NOISE_LAYER_1; x++){
		for(int y = 0; y < NOISE_LAYER_1; y++){
			for(int z = 0; z < NOISE_LAYER_1; z++){
				noiseLayer1[x][y][z] = NoiseFunc::perlinNoise3D(samplePoint.x+x,samplePoint.y+y,samplePoint.z+z,8,0.25);
			}
		}
	}

	samplePoint.x = (double)(rand()%10000 * 0.001) * (rand()%1000);
	samplePoint.y = (double)(rand()%10000 * 0.001) * (rand()%1000);
	samplePoint.z = (double)(rand()%10000 * 0.001) * (rand()%1000);

	for(int x = 0; x < NOISE_LAYER_2; x++){
		for(int y = 0; y < NOISE_LAYER_2; y++){
			for(int z = 0; z < NOISE_LAYER_2; z++){
				noiseLayer2[x][y][z] = NoiseFunc::perlinNoise3D(samplePoint.x+x,samplePoint.y+y,samplePoint.z+z,8,0.25);
			}
		}
	}

	samplePoint.x = (double)(rand()%10000 * 0.001) * (rand()%1000);
	samplePoint.y = (double)(rand()%10000 * 0.001) * (rand()%1000);
	samplePoint.z = (double)(rand()%10000 * 0.001) * (rand()%1000);
	
	for(int x = 0; x < NOISE_LAYER_3; x++){
		for(int y = 0; y < NOISE_LAYER_3; y++){
			for(int z = 0; z < NOISE_LAYER_3; z++){
				noiseLayer3[x][y][z] = NoiseFunc::perlinNoise3D(samplePoint.x+x,samplePoint.y+y,samplePoint.z+z,8,0.25);
			}
		}
	}

}

double TerrainChunk::fetchNoise(Vector3 pos){
	int x,y,z;
	x = (int)pos.x;
	y = (int)pos.y;
	z = (int)pos.z;

	if(x<0) x*=-1;
	if(y<0) y*=-1;
	if(z<0) z*=-1;

	double total = 0;


	total+= noiseLayer1[x%NOISE_LAYER_1][y%NOISE_LAYER_1][z%NOISE_LAYER_1];
	total+= noiseLayer2[x%NOISE_LAYER_2][y%NOISE_LAYER_2][z%NOISE_LAYER_2];
	total+= noiseLayer3[x%NOISE_LAYER_3][y%NOISE_LAYER_3][z%NOISE_LAYER_3];

	return total;
}