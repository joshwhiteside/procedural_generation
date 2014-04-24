#pragma once

#include "Mesh.h"



class JuliaSet : public Mesh {
public:
	JuliaSet() : Mesh() {c = 0.0f;};
	~JuliaSet() {};

	inline void SetcolorMap(GLuint tex)	{colorMap = tex;}
	inline GLuint GetcolorMap()			{return colorMap;}


	virtual void Draw();

	void Update() {c++;}

	float getCycle() { return c;}

protected:
	GLuint colorMap;
	float c;
};