#pragma once

#include "OGLRenderer.h"

#define SHADER_VERTEX		0
#define SHADER_FRAGMENT		1
#define SHADER_GEOMETRY		2
#define	SHADER_CONTROL		3
#define SHADER_EVALUATION	4

using namespace std;
class Shader	{
public:
	Shader(string vertex, string fragment , string geometry = "",string tessControl = "", string tessEval= "");
	~Shader(void);

	GLuint  GetProgram() { return program;}
	bool	LinkProgram();

	Shader(Shader * shader);

	void InitShader(string vertex, string fragment , string geometry, string tessControl, string tessEval);
		
	string fFile;
	string vFile;
	string gFile;
	string tcFile;
	string teFile;

protected:
	bool	LoadShaderFile(string from, string &into);
	GLuint	GenerateShader(string from, GLenum type);
	void	SetDefaultAttributes();

	bool    ReturnNewProgram(GLuint& prog);

	GLuint objects[3];
	GLuint program;


	

	bool loadFailed;
};