#pragma once
#include "OGLRenderer.h"

enum MeshBuffer{
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER,
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER,
	MAX_BUFFER
};

class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();

	inline void SetTexture(GLuint tex)	{texture = tex;}
	inline GLuint GetTexture()			{return texture;}

	inline void SetTexture2(GLuint tex)	{texture2 = tex;}
	inline GLuint GetTexture2()			{return texture2;}

	void GenerateNormals();

	void SetBumpMap(GLuint tex)			{bumpTexture = tex;}
	GLuint GetBumpMap()					{return bumpTexture;}

	void GenerateColours();

	inline void SetType(GLuint t)	{type = t;}
	inline GLuint GetType()			{return type;}

protected:
	virtual void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;

	GLuint texture;
	Vector2* textureCoords;

	GLuint texture2;
	Vector2* texture2Coords;

	float cycle;

	GLuint numIndices;
	unsigned int* indices;

	Vector3* normals;

	
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
							const Vector3 &c, const Vector2 &ta,
							const Vector2 &tb,const Vector2 &tc);

	Vector3*	tangents;
	GLuint	bumpTexture;
};