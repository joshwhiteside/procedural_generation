#pragma once
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"


class CubeRobot : public SceneNode	{
public:
	CubeRobot(Shader * shader);
	CubeRobot();
	~CubeRobot(void){};
	virtual void		Update(float msec);

	static void CreateCube()	{
		OBJMesh*m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		cube = m;
	}

	static void DeleteCube()	{delete cube;}

	virtual void Draw(OGLRenderer &r);

	static Mesh* cube;

	SceneNode* GetHead() {return head;}

protected:
	
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
	//SceneNode* eye;
	SceneNode* leftEye;
	SceneNode* rightEye;
	float angle;
};