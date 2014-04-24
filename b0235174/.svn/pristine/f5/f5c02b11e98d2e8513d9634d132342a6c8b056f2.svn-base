#pragma once

#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "CubeRobot.h"


class RobotEye: public SceneNode
{
public:
	RobotEye(Shader * shader);
	~RobotEye(void);


	virtual void	PreDraw(OGLRenderer &r);
	virtual void				SetSpecificUniforms(OGLRenderer &r);


	float lazerMin;
	float lazerMax;

};

