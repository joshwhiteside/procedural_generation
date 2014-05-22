#include "noisefunc.h"

GLuint NoiseFunc::generate3DNoiseTexture(int xSize, int ySize, int zSize) {
	GLuint tex;

	std::vector<unsigned char> values;


	for(int x = 0; x < xSize; x++){
		for(int y= 0; y < ySize; y++) {
			for(int z = 0; z < zSize; z++) {
				//double tempNoiseVal = getNoise3(x,y,z);
				double tempNoiseVal = perlinNoise3D(x,y,z,9,0.75);
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
	
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, xSize, ySize, zSize, 0,GL_RED, GL_UNSIGNED_BYTE, &(*values.begin()));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	

	glBindTexture(GL_TEXTURE_3D,0);
	
	return tex;

}



double NoiseFunc::perlinNoise3D(double x, double y, double z, int octaves, double persistence){
	double total = 0;

	for(int i = 0; i < octaves; i++){
		double frequency = pow(2,(double)i);
		double amplitude = pow(persistence,(double)i);
		

		
		double temp = getNoise3Hermite(x*frequency,y*frequency,z*frequency,1,0,0) * amplitude;
		total += temp;
		//cout <<"Octave: " << i << "Total : " << total << "TEMP: " << temp <<  endl;
	}


	return total;
}

 double NoiseFunc::getNoise3(double x, double y, double z) {
		double floorX = (double)((int) x);
		double floorY = (double)((int) y);
		double floorZ = (double)((int) z);

		double a,b,c,d,e,f,g,h;

		a = findNoise3(floorX, floorY, floorZ);
		b = findNoise3(floorX + 1, floorY, floorZ);

		c = findNoise3(floorX, floorY+1, floorZ);
		d = findNoise3(floorX + 1, floorY+1, floorZ);

		e = findNoise3(floorX, floorY, floorZ+1);
		f = findNoise3(floorX + 1, floorY, floorZ+1);

		g = findNoise3(floorX, floorY+1, floorZ+1);
		h = findNoise3(floorX + 1, floorY+1, floorZ+1);

		double int1 = interpolate(a,b,x-floorX);
		double int2 = interpolate(c,d,x-floorX);

		double int3 = interpolate(e,f, x-floorX);
		double int4 = interpolate(g,h,x-floorX);

		double int21 = interpolate(int1, int2, y-floorY);
		double int22 = interpolate(int3, int4, y-floorY);

		return interpolate(int21,int22, z-floorZ);
		//return 0.5;

	}

double  NoiseFunc::hermiteInterpolation(double y0,double y1,double y2,double y3, //Four noise values of points
										double mu,								 //Vague bias... i think?
										double tension,							 //Tension!
										double bias){							 //Other thing
	double m0,m1;
	double mu2,mu3;
	double a0,a1,a2,a3;

	mu2 = mu * mu;
	mu3 = mu * mu2;

	m0 = (y1-y0) * (1+bias) * (1-tension) * 0.5;
	m0+= (y2-y1) * (1-bias) * (1-tension) * 0.5;

	m1 = (y2-y1) * (1+bias) * (1-tension) * 0.5;
	m1+= (y3-y2) * (1-bias) * (1-tension) * 0.5;

	a0 = 2*mu3 - 3*mu2 +1;
	a1 = mu3 - 2*mu2 + mu;
	a2 = mu3 - mu2;
	a3 =2*mu3 + 3 * mu2;

	return( (a0 * y1) + (a1 * m0) + (a2 * m1) + (a3 * y2));
}

double  NoiseFunc::getNoise3Hermite(double x, double y, double z, double mu, double tension, double bias){
		double floorX = (double)((int) x);
		double floorY = (double)((int) y);
		double floorZ = (double)((int) z);

		double a,b,c,d,e,f,g,h;

		a = findNoise3(floorX, floorY, floorZ);
		b = findNoise3(floorX + 1, floorY, floorZ);

		c = findNoise3(floorX, floorY+1, floorZ);
		d = findNoise3(floorX + 1, floorY+1, floorZ);

		e = findNoise3(floorX, floorY, floorZ+1);
		f = findNoise3(floorX + 1, floorY, floorZ+1);

		g = findNoise3(floorX, floorY+1, floorZ+1);
		h = findNoise3(floorX + 1, floorY+1, floorZ+1);

		//double int1 = hermiteInterpolation(a,b,c,d,mu,tension,bias);
		//double int2 = hermiteInterpolation(e,f,g,h,mu,tension,bias);

		double int1 = interpolate(a,b,mu);
		double int2 = interpolate(c,d,mu);

		double int3 = interpolate(e,f,mu);
		double int4 = interpolate(g,h,mu);

		double int21 = interpolate(int1,int2,mu);
		double int22 = interpolate(int3,int4,mu);

		//cout << "INT 1: " << int21 << "INT2: " << int22 << endl;
		//cout << "Noises: " << a << endl;

		return interpolate(int21,int22,mu);

}