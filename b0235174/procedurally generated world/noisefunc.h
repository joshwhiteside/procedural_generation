#pragma once

#include <math.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")

#include "GL/glew.h"
#include "GL/wglew.h"
#include <vector>
#include <iostream>


using namespace std;

class NoiseFunc {
public:

	//These functions return noise in the range -1 to 1
	static inline double findNoise(double x) {
		int n = (int) x;
		n = (n<<13 ^ n);
		return (double) (1.0 - ( ( n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
	}

	static inline double findNoise2(double x, double y){
		int n=(int)x+(int)y*57;
		n=(n<<13)^n;
		int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
		return 1.0-((double)nn/1073741824.0);
	}

	static double findNoise3(double x,double y,double z) {
		//return findNoise(z + findNoise2(x,y));
		int n=(int)x+(int)y*59+(int)z*443;
		n=(n<<13)^n;
		int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
		return 1.0-((double)nn/1073741824.0);

	}

	//Cosine interpolation
	static inline double interpolate(double a, double b, double x) {
		double ft=x * 3.1415927;
		double f=(1.0-cos(ft))* 0.5;
		return a*(1.0-f)+b*f;
	}

	static double hermiteInterpolation(double y0,double y1,double y2,double y3,
										double mu,
										double tension,
										double bias);

	static double getNoise(double x, double y){
		double floorX = (double)((int) x);
		double floorY = (double)((int) y);
		double s,t,u,v;

		s= findNoise2(floorX,floorY);
		t= findNoise2(floorX+1, floorY);
		u= findNoise2(floorX, floorY + 1);
		v= findNoise2(floorX+1 , floorY + 1);

		double int1 = interpolate(s,t,x-floorX);
		double int2 = interpolate(u,v,x-floorX);
		return interpolate(int1,int2,y-floorY);
	}

	static double getNoise3(double x, double y, double z);

	static double perlinNoise3D(double x, double y, double z, int octaves, double persistence);

	static double getNoise3Hermite(double x, double y, double z, double mu, double tension, double bias);

	static GLuint generate3DNoiseTexture(int xSize, int ySize, int zSize);

};