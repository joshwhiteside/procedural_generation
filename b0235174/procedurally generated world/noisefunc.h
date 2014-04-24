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
		return findNoise(z + findNoise2(x,y));
	}

	static inline double interpolate(double a, double b, double x) {
		double ft=x * 3.1415927;
		double f=(1.0-cos(ft))* 0.5;
		return a*(1.0-f)+b*f;
	}

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

	static double getNoise3(double x, double y, double z) {
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

	static GLuint generate3DNoiseTexture(int xSize, int ySize, int zSize);

};