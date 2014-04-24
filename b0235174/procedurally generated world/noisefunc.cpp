#include "noisefunc.h"

GLuint NoiseFunc::generate3DNoiseTexture(int xSize, int ySize, int zSize) {
	GLuint tex;

	std::vector<unsigned char> values;


	for(int x = 0; x < xSize; x++){
		for(int y= 0; y < ySize; y++) {
			for(int z = 0; z < zSize; z++) {
				double tempNoiseVal = getNoise3(x,y,z);
				unsigned char toPut = (unsigned char)(((tempNoiseVal+1) * 0.5) * 255.0); //Perlin noise in the range 0 to 255.
				//unsigned int toPut = 128;
				
				values.push_back(toPut);
				//cout << toPut << endl;
			}
		}
	}
	
	glGenTextures(1, &tex);
	
	glBindTexture(GL_TEXTURE_3D, tex);
	//glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, xSize, ySize, zSize, 0,GL_RED, GL_UNSIGNED_BYTE, &(*values.begin()));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	

	glBindTexture(GL_TEXTURE_3D,0);
	
	return tex;

}

